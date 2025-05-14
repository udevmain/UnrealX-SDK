#include "netkit/player/UnrealXOnlinePlayerController.h"
#include "netkit/player/UnrealXGameInstance.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LatentActionManager.h"

class FGetUserIDLatentAction : public FPendingLatentAction
{
public:
    FString& OutUserID;
    bool& bSuccess;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;
    bool bIsCompleted;

    FGetUserIDLatentAction(FString& InOutUserID, bool& InOutSuccess, const FLatentActionInfo& LatentInfo)
        : OutUserID(InOutUserID)
        , bSuccess(InOutSuccess)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
        , bIsCompleted(false)
    {
        // Initiate the HTTP request here
        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(TEXT("http://unrealx.space/gameApi/getUserID"));
        Request->SetVerb("GET");

        // Retrieve the token from the GameInstance
        if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(LatentInfo.CallbackTarget))
        {
            if (UUnrealXGameInstance* GI = World->GetGameInstance<UUnrealXGameInstance>())
            {
                FString Token = GI->GetToken();
                Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *Token));
            }
        }

        Request->OnProcessRequestComplete().BindRaw(this, &FGetUserIDLatentAction::OnResponseReceived);
        Request->ProcessRequest();
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(bIsCompleted, ExecutionFunction, OutputLink, CallbackTarget);
    }

    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        if (bWasSuccessful && Response.IsValid())
        {
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                OutUserID = JsonObject->GetStringField("userID");
                bSuccess = true;
            }
            else
            {
                bSuccess = false;
            }
        }
        else
        {
            bSuccess = false;
        }

        bIsCompleted = true;
    }
};

void AUnrealXOnlinePlayerController::GetUserID(UObject* WorldContextObject, FLatentActionInfo LatentInfo, FString& OutUserID, bool& bSuccess)
{
    if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
    {
        FLatentActionManager& LatentManager = World->GetLatentActionManager();
        if (LatentManager.FindExistingAction<FGetUserIDLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
        {
            LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FGetUserIDLatentAction(OutUserID, bSuccess, LatentInfo));
        }
    }
}