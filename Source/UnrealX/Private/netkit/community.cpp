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
        FString URL = FString::Printf(TEXT("https://unrealx.space/api/getUser?appID=%s&platform=%s&platformID=%s"),
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
        FString URL = FString::Printf(TEXT("http://localhost:3000/api/getExtraDataValue?userID=%s&identifier=%s"),
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
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bCompleted = false;

    FCheckUserExistsAction(const FString& AppID, const FString& Platform, const FString& PlatformID,
        bool& Result, const FLatentActionInfo& LatentInfo)
        : bOutExists(Result), ExecutionFunction(LatentInfo.ExecutionFunction),
        OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
    {
        FString URL = FString::Printf(TEXT("https://unrealx.space/api/checkUserExists?appID=%s&platform=%s&platformID=%s"),
            *FGenericPlatformHttp::UrlEncode(AppID), *FGenericPlatformHttp::UrlEncode(Platform), *FGenericPlatformHttp::UrlEncode(PlatformID));

        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(URL);
        Request->SetVerb("GET");

        Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                bOutExists = bSuccess && Response->GetResponseCode() == 200 && Response->GetContentAsString() == "1";
                bCompleted = true;
            });

        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UCommunity::CheckUserExists(const FString& AppID, const FString& Platform, const FString& PlatformID, const FLatentActionInfo LatentInfo, bool& bExists)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (!LatentManager.FindExistingAction<FCheckUserExistsAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                new FCheckUserExistsAction(AppID, Platform, PlatformID, bExists, LatentInfo));
        }
    }
}

void UCommunity::AddUser(const FString& AppID, const FString& UserID, const FString& Platform, const FString& PlatformID, const FString& ExtraDataJson)
{
    FString URL = FString::Printf(TEXT("https://unrealx.space/api/addUser?appID=%s&userID=%s&platform=%s&platformID=%s&extra_data=%s"),
        *FGenericPlatformHttp::UrlEncode(AppID),
        *FGenericPlatformHttp::UrlEncode(UserID),
        *FGenericPlatformHttp::UrlEncode(Platform),
        *FGenericPlatformHttp::UrlEncode(PlatformID),
        *FGenericPlatformHttp::UrlEncode(ExtraDataJson));

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->ProcessRequest(); // Fire-and-forget
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
        FString URL = FString::Printf(TEXT("http://localhost:3000/api/updateUserExtraData?userID=%s&extra_data=%s"),
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

