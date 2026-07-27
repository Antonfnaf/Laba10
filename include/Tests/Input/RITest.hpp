#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "UIEngine/input/RawInput.h"
#include "Core/enums.h"

class RITest {
public:
    static void Start() {
        RawInput input;
        input.EnableRawMode();
        
        std::cout << "=== Raw Input Test ===\n";
        std::cout << "Press keys to see their codes. Press 'q' to quit.\n\n";

        bool running = true;
        while (running) {
            std::vector<Key> keys = input.GetKeyCodes();
            
            if (!keys.empty()) {
                for (size_t i = 0; i < keys.size(); ++i) {
                    std::cout << GetKeyName(keys[i]);
                    if (i < keys.size() - 1) std::cout << "\t";
                    
                    if (keys[i] == Key::q) {
                        running = false;
                    }
                }
                std::cout << "\n";
            }
            
            // Небольшая задержка, чтобы не грузить процессор на 100%
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        input.DisableRawMode();
        std::cout << "Test finished.\n";
    }
};