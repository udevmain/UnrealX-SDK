#include "SolanaManager.h"
#include "HttpModule.h"
#include "IHttpRequest.h"
#include "Json.h"
#include "solana.hpp" // solcpp

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
