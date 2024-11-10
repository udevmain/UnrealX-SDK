#include "Core/sdk_subsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"

FString Usdk_subsystem::GetUserData(EUserData Type)
{
    FString TypeString;
    switch (Type)
    {
    case EUserData::ID:
        TypeString = "id";
        break;
    case EUserData::Email:
        TypeString = "email";
        break;
    case EUserData::Username:
        TypeString = "username";
        break;
    case EUserData::WalletAddress:
        TypeString = "wallet_address";
        break;
    default:
        TypeString = "";
        break;
    }

    FString Url = FString::Printf(
        TEXT("https://dev.unrealx_sdk.com/api/%s/getUserData.php?type=%s&bareer=%s&DBName=%s"),
        *AppName.ToString(),
        *TypeString,
        *ApiKey,
        *GenDBName.ToString()
    );

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FString ResponseContent = Response->GetContentAsString();
                UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseContent);
                return ResponseContent;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to get user data!"));
            }
        });

    Request->ProcessRequest();
}
