#include "async/CreateUserSessionAsync.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "netkit/player/UnrealXGameInstance.h"

UCreateUserSessionAsync* UCreateUserSessionAsync::CreateUserSession(const FString& InAppID, const FString& InPlatform, const FString& InPlatformID)
{
	UCreateUserSessionAsync* Node = NewObject<UCreateUserSessionAsync>();
	Node->AppID = InAppID;
	Node->Platform = InPlatform;
	Node->PlatformID = InPlatformID;
	return Node;
}

void UCreateUserSessionAsync::Activate()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("http://unrealx.space/gameApi/auth/login")); // Change this URL to match your backend
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
	Json->SetStringField("appID", AppID);
	Json->SetStringField("platform", Platform);
	Json->SetStringField("platformID", PlatformID);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	Request->SetContentAsString(OutputString);
	Request->OnProcessRequestComplete().BindUObject(this, &UCreateUserSessionAsync::HandleResponse);
	Request->ProcessRequest();
}

void UCreateUserSessionAsync::HandleResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnFail.Broadcast();
		return;
	}

	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, Json) && Json.IsValid())
	{
		const FString Token = Json->GetStringField("token");
		const FString UserID = Json->GetStringField("userID");

		// Save to GameInstance
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(this))
		{
			if (UUnrealXGameInstance* GI = World->GetGameInstance<UUnrealXGameInstance>())
			{
				GI->SetToken(Token);
				GI->SetUserID(UserID);
			}
		}

		OnSuccess.Broadcast(Token, UserID);
	}
	else
	{
		OnFail.Broadcast();
	}
}
