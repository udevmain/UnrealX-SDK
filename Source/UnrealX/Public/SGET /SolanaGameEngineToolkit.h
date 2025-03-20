#pragma once

#include <iostream>

class SolanaGameEngineToolkit {
public:
    void MintNFT(const std::string& nftName) {
        std::cout << "Minting NFT: " << nftName << " on Solana..." << std::endl;
    }

    void MakeTransaction(double amount) {
        std::cout << "Sending transaction of " << amount << " SOL..." << std::endl;
    }
};
