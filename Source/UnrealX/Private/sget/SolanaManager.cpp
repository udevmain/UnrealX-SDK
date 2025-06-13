#include "sget/SolanaManager.h"
#include "HttpModule.h"
#include "IHttpRequest.h"
#include "Json.h"
#include "solana.hpp"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void USolanaManager::Init(ESolanaNetwork Network) {
  RpcUrl = Network == ESolanaNetwork::MainnetBeta
    ? TEXT("https://api.mainnet-beta.solana.com")
    : TEXT("https://api.testnet.solana.com");
  Connection = new solana::rpc::Connection(TCHAR_TO_UTF8(*RpcUrl));
}

FString USolanaManager::GetWalletAddress(const FString& PrivateKeyBase58) {
  auto signer = solana::Keypair::fromBase58(TCHAR_TO_UTF8(*PrivateKeyBase58));
  return UTF8_TO_TCHAR(signer.publicKey.toBase58().c_str());
}

bool USolanaManager::SendTransaction(const FString& FromPriv, const FString& ToPub, float AmountSOL, FString& OutSig) {
  try {
    auto from = solana::Keypair::fromBase58(TCHAR_TO_UTF8(*FromPriv));
    auto to   = solana::PublicKey::fromBase58(TCHAR_TO_UTF8(*ToPub));
    uint64_t lam = uint64_t(AmountSOL * 1e9);
    auto recent = Connection->getLatestBlockhash();
    solana::Transaction tx;
    tx.feePayer = from.publicKey;
    tx.recentBlockhash = recent.blockhash;
    tx.addInstruction(solana::system_program::transfer(from.publicKey, to, lam));
    tx.sign({&from});
    auto sig = Connection->sendTransaction(tx);
    OutSig = UTF8_TO_TCHAR(sig.c_str());
    return true;
  } catch(...) {
    return false;
  }
}

bool USolanaManager::BatchTransaction(const FString& FromPriv, const TArray<FString>& ToPubs, float AmountPerAddr, TArray<FString>& OutSigs) {
  OutSigs.Empty();
  for (auto& toPub : ToPubs) {
    FString sig;
    if (SendTransaction(FromPriv, toPub, AmountPerAddr, sig))
      OutSigs.Add(sig);
    else
      OutSigs.Add(TEXT("ERROR"));
  }
  return true;
}

bool USolanaManager::GetBalance(const FString& PubKey, double& OutBalance) {
  try {
    auto balLam = Connection->getBalance(TCHAR_TO_UTF8(*PubKey)).value;
    OutBalance = (double)balLam / 1e9;
    return true;
  } catch(...) { return false; }
}

bool USolanaManager::GetWalletNFTs(const FString& WalletAddress, TArray<FSolanaNFT>& OutNFTs) {
  OutNFTs.Empty();
  return true;
}

void USolanaManager::GetWalletNFTsAsync(const FString& WalletAddress)
{
    InternalFetchNFTs(WalletAddress);
}

void USolanaManager::InternalFetchNFTs(const FString& WalletAddress)
{
    // Token Metadata Program ID (Metaplex)
    FString ProgramId = TEXT("metaqbxxUerdq28cj1RbAWkYQm3ybzjb6a8bt518x1s"); 

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(RpcUrl);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");

    FString JsonBody = FString::Printf(TEXT(R"({
        "jsonrpc":"2.0",
        "id":1,
        "method":"getProgramAccounts",
        "params":[
            "%s",
            {
                "encoding":"jsonParsed",
                "filters":[
                    {"memcmp":{"offset":32,"bytes":"%s"}},
                    {"dataSize":165}
                ]
            }
        ]
    })"), *ProgramId, *WalletAddress);

    Request->SetContentAsString(JsonBody);

    Request->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
        {
            if (!bSuccess || !Resp.IsValid()) return;

            TSharedPtr<FJsonObject> RootObj;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Resp->GetContentAsString());

            if (FJsonSerializer::Deserialize(Reader, RootObj) && RootObj.IsValid())
            {
                const TArray<TSharedPtr<FJsonValue>>* Accounts;
                if (RootObj->GetObjectField("result")->TryGetArrayField("value", Accounts))
                {
                    for (auto& Value : *Accounts)
                    {
                        TSharedPtr<FJsonObject> Obj = Value->AsObject();
                        FString Mint = Obj->GetObjectField("account")->GetObjectField("data")->GetObjectField("parsed")->GetObjectField("info")->GetStringField("mint");

                        // Генерация URI через стандартный PDA
                        FString MetadataUri = FString::Printf(TEXT("https://api-mainnet.magiceden.dev/v2/tokens/%s"), *Mint);

                        PendingNFTs.Add(Mint, TEXT("Unknown"));

                        TSharedRef<IHttpRequest> MetaReq = FHttpModule::Get().CreateRequest();
                        MetaReq->SetURL(MetadataUri);
                        MetaReq->SetVerb("GET");
                        MetaReq->OnProcessRequestComplete().BindUObject(this, &USolanaManager::OnNFTMetadataResponse, Mint, TEXT("NFT"));
                        MetaReq->ProcessRequest();
                    }
                }
            }
        });

    Request->ProcessRequest();
}

void USolanaManager::OnNFTMetadataResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString MintAddress, FString NameHint)
{
    if (!bWasSuccessful || !Response.IsValid()) return;

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FSolanaNFT NFT;
        NFT.MintAddress = MintAddress;
        NFT.Name = JsonObject->GetStringField("name");
        NFT.URI = JsonObject->GetStringField("image");

        UE_LOG(LogTemp, Log, TEXT("NFT: %s (%s) => %s"), *NFT.Name, *MintAddress, *NFT.URI);
    }

    PendingNFTs.Remove(MintAddress);
}

