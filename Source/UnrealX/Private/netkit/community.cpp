#include "netkit/community.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LatentActionManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

class FGetUserDataAction : public FPendingLatentAction
{
public:
    FGameUserData& OutData;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FGetUserDataAction(const FString& AppID, const FString& Platform, const FString& PlatformID,
        FGameUserData& Result,
        const FLatentActionInfo& LatentInfo)
        : OutData(Result)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
    {
        FString URL = FString::Printf(TEXT("https://unrealx.space/gameApi/getUser?appID=%s&platform=%s&platformID=%s"),
            *FGenericPlatformHttp::UrlEncode(AppID), *FGenericPlatformHttp::UrlEncode(Platform), *FGenericPlatformHttp::UrlEncode(PlatformID));

        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(URL);
        Request->SetVerb("GET");
        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                if (bSuccess && Response->GetResponseCode() == 200)
                {
                    TSharedPtr<FJsonObject> JsonObject;
                    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

                    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
                    {
                        OutData.ID = JsonObject->GetIntegerField("id");
                        OutData.UserID = JsonObject->GetStringField("userID");
                        OutData.ExtraData = JsonObject->GetStringField("extra_data");
                        OutData.CreatedAt = JsonObject->GetStringField("created_at");
                    }
                }

                bCompleted = true;
            });

        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::GetUserData(const FString& AppID, const FString& Platform, const FString& PlatformID, const FLatentActionInfo LatentInfo, FGameUserData& OutData)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FGetUserDataAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FGetUserDataAction(AppID, Platform, PlatformID, OutData, LatentInfo));
        }
    }
}

class FGetExtraDataValueAction : public FPendingLatentAction
{
public:
    FString& OutValue;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FGetExtraDataValueAction(const FString& UserID, const FString& Identifier,
        FString& Result, const FLatentActionInfo& LatentInfo)
        : OutValue(Result), ExecutionFunction(LatentInfo.ExecutionFunction),
        OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
    {
        FString URL = FString::Printf(TEXT("https://unrealx.space/gameApi/getExtraDataValue?userID=%s&identifier=%s"),
            *FGenericPlatformHttp::UrlEncode(UserID), *FGenericPlatformHttp::UrlEncode(Identifier));

        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(URL);
        Request->SetVerb("GET");

        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                if (bSuccess && Response->GetResponseCode() == 200)
                {
                    OutValue = Response->GetContentAsString();
                }
                bCompleted = true;
            });

        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::GetExtraDataValue(const FString& UserID, const FString& Identifier, const FLatentActionInfo LatentInfo, FString& OutValue)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FGetExtraDataValueAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FGetExtraDataValueAction(UserID, Identifier, OutValue, LatentInfo));
        }
    }
}

class FCheckUserExistsAction : public FPendingLatentAction
{
public:
    bool& bOutExists;
    FString& OutDebugInfo; // Добавлено для отладки
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FCheckUserExistsAction(const FString& AppID, const FString& Platform, const FString& PlatformID,
        bool& Result, FString& DebugInfo, const FLatentActionInfo& LatentInfo)
        : bOutExists(Result), OutDebugInfo(DebugInfo), ExecutionFunction(LatentInfo.ExecutionFunction),
        OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
    {
        // Формируем URL с кодированием параметров
        FString URL = FString::Printf(TEXT("https://unrealx.space/gameApi/userExists?appID=%s&platform=%s&platformID=%s"),
            *FGenericPlatformHttp::UrlEncode(AppID), *FGenericPlatformHttp::UrlEncode(Platform), *FGenericPlatformHttp::UrlEncode(PlatformID));

        OutDebugInfo += FString::Printf(TEXT("[Request] URL: %s\n"), *URL);
        UE_LOG(LogTemp, Warning, TEXT("CheckUserExists Request URL: %s"), *URL);

        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(URL);
        Request->SetVerb("GET");
        Request->SetTimeout(10); // 10 секунд таймаут

        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                FString DebugMessage;
                int32 ResponseCode = Response.IsValid() ? Response->GetResponseCode() : 0;
                FString Content = Response.IsValid() ? Response->GetContentAsString() : TEXT("Invalid response");

                DebugMessage += FString::Printf(TEXT("[Response] Success: %d, Code: %d, Content: %s\n"),
                    bSuccess, ResponseCode, *Content);

                // Детальный анализ ответа
                if (!bSuccess)
                {
                    DebugMessage += TEXT("[Error] Request failed completely\n");
                }
                else if (!Response.IsValid())
                {
                    DebugMessage += TEXT("[Error] Response is invalid\n");
                }
                else if (ResponseCode != 200)
                {
                    DebugMessage += FString::Printf(TEXT("[Error] HTTP %d: %s\n"),
                        ResponseCode, *Response->GetContentAsString());
                }

                bOutExists = bSuccess && Response.IsValid() && ResponseCode == 200 && Content.Equals("1");
                DebugMessage += FString::Printf(TEXT("[Result] User exists: %d\n"), bOutExists);

                OutDebugInfo = DebugMessage;
                UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMessage);
                bCompleted = true;
            });

        // Логирование начала запроса
        UE_LOG(LogTemp, Warning, TEXT("Starting CheckUserExists request..."));
        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::CheckUserExists(const FString& AppID, const FString& Platform, const FString& PlatformID,
    const FLatentActionInfo LatentInfo, bool& bExists, FString& OutDebugInfo)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FCheckUserExistsAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FCheckUserExistsAction(AppID, Platform, PlatformID, bExists, OutDebugInfo, LatentInfo));
        }
    }
}

class FAddUserAction : public FPendingLatentAction
{
public:
    bool& bOutSuccess;
    FString& OutUserID;
    FString& OutResponse; // Новое поле для хранения ответа сервера
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FAddUserAction(const FString& AppID, const FString& Platform, const FString& PlatformID, const FString& ExtraDataJson,
        bool& Result, FString& GeneratedUserID, FString& ServerResponse, const FLatentActionInfo& LatentInfo)
        : bOutSuccess(Result), OutUserID(GeneratedUserID), OutResponse(ServerResponse),
        ExecutionFunction(LatentInfo.ExecutionFunction),
        OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
    {
        // Генерация userID
        const FString Characters = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        FString RandomID;
        for (int32 i = 0; i < 16; ++i)
        {
            int32 Index = FMath::RandRange(0, Characters.Len() - 1);
            RandomID.AppendChar(Characters[Index]);
        }
        OutUserID = "unrealX-ply-" + RandomID;

        // Создание JSON тела
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        JsonObject->SetStringField("appID", AppID);
        JsonObject->SetStringField("userID", OutUserID);
        JsonObject->SetStringField("platform", Platform);
        JsonObject->SetStringField("platformID", PlatformID);
        JsonObject->SetStringField("extra_data", ExtraDataJson);

        FString RequestBody;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

        // HTTP POST
        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL("https://unrealx.space/gameApi/addUser");
        Request->SetVerb("POST");
        Request->SetHeader("Content-Type", "application/json");
        Request->SetContentAsString(RequestBody);

        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
            {
                if (!bSuccess || !Resp.IsValid())
                {
                    OutResponse = FString::Printf(TEXT("Request failed. Success: %d, Valid response: %d"),
                        bSuccess, Resp.IsValid());
                    bOutSuccess = false;
                }
                else
                {
                    OutResponse = Resp->GetContentAsString();

                    // Проверяем код ответа и содержимое
                    const int32 ResponseCode = Resp->GetResponseCode();
                    bOutSuccess = (ResponseCode == 200);

                    if (!bOutSuccess)
                    {
                        OutResponse = FString::Printf(TEXT("HTTP %d: %s"),
                            ResponseCode, *OutResponse);
                    }
                }

                // Логируем полный ответ для отладки
                UE_LOG(LogTemp, Warning, TEXT("AddUser Response: %s"), *OutResponse);
                bCompleted = true;
            });

        // Логируем отправляемый запрос
        UE_LOG(LogTemp, Warning, TEXT("Sending AddUser request: %s"), *RequestBody);
        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::AddUser(const FString& AppID, const FString& Platform, const FString& PlatformID, const FString& ExtraDataJson,
    const FLatentActionInfo LatentInfo, bool& bSuccess, FString& OutUserID, FString& OutResponse)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FAddUserAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FAddUserAction(AppID, Platform, PlatformID, ExtraDataJson,
                    bSuccess, OutUserID, OutResponse, LatentInfo));
        }
    }
}

class FUpdateUserExtraDataAction : public FPendingLatentAction
{
public:
    bool& bOutSuccess;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FUpdateUserExtraDataAction(const FString& UserID, const FString& ExtraDataJson,
        bool& Result, const FLatentActionInfo& LatentInfo)
        : bOutSuccess(Result), ExecutionFunction(LatentInfo.ExecutionFunction),
        OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
    {
        FString URL = FString::Printf(TEXT("http://unrealx.space/gameApi/updateUserExtraData?userID=%s&extra_data=%s"),
            *FGenericPlatformHttp::UrlEncode(UserID),
            *FGenericPlatformHttp::UrlEncode(ExtraDataJson));

        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(URL);
        Request->SetVerb("GET");

        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                bOutSuccess = bSuccess && Response->GetResponseCode() == 200;
                bCompleted = true;
            });

        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::UpdateUserExtraData(const FString& UserID, const FString& ExtraDataJson, const FLatentActionInfo LatentInfo, bool& bSuccess)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FUpdateUserExtraDataAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FUpdateUserExtraDataAction(UserID, ExtraDataJson, bSuccess, LatentInfo));
        }
    }
}

