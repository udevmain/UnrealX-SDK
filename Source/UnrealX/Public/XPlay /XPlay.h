#pragma once

#include <iostream>

class XPlay {
public:
    void MoveCharacter(int x, int y) {
        std::cout << "Moving character to position: " << x << ", " << y << std::endl;
    }

    void InteractWithObject(const std::string& object) {
        std::cout << "Interacting with object: " << object << std::endl;
    }
};
