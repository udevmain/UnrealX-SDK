#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SolanaManager.generated.h"

USTRUCT(BlueprintType)
struct FSolanaNFT { GENERATED_BODY()
  UPROPERTY(BlueprintReadOnly) FString Name;
  UPROPERTY(BlueprintReadOnly) FString MintAddress;
  UPROPERTY(BlueprintReadOnly) FString URI;
};

UENUM(BlueprintType)
enum class ESolanaNetwork : uint8 {
  MainnetBeta UMETA(DisplayName="Mainnet Beta"),
  Testnet     UMETA(DisplayName="Testnet")
};

UCLASS(Blueprintable)
class UNREALX_API USolanaManager : public UObject {
  GENERATED_BODY()

private:
  solana::rpc::Connection* Connection = nullptr;
  FString RpcUrl;

public:
  UFUNCTION(BlueprintCallable, Category="Solana")
  void Init(ESolanaNetwork Network);

  UFUNCTION(BlueprintCallable, Category="Solana")
  FString GetWalletAddress(const FString& PrivateKeyBase58);

  UFUNCTION(BlueprintCallable, Category="Solana")
  bool SendTransaction(const FString& FromPriv, const FString& ToPub, float AmountSOL, FString& OutSig);

  UFUNCTION(BlueprintCallable, Category="Solana")
  bool BatchTransaction(const FString& FromPriv, const TArray<FString>& ToPubs, float AmountPerAddr, TArray<FString>& OutSigs);

  UFUNCTION(BlueprintCallable, Category="Solana")
  bool GetBalance(const FString& PubKey, double& OutBalance);

  UFUNCTION(BlueprintCallable, Category="Solana")
  bool GetWalletNFTs(const FString& WalletAddress, TArray<FSolanaNFT>& OutNFTs);
};
