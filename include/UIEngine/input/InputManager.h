#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
/*
#include <conio.h>*/


#include "Core/enums.h"
#include "UIEngine/input//Bind.h"
//#include "Core/Pixel.h"


struct InputManager
{
    
private:
    static Bind actions;
    static KeyCode getKeyCode();

public:
    /////////////
    static void Init();      // Запускает поток ввода
    static void Shutdown();
    /////////////
    /*inline static bool hasKey() {
        return _kbhit() != 0;
    }*/
    static void Bind(KeyCode key, std::function<void()> action);
    static void Bind(std::map < KeyCode, std::function<void()>> localActions);
    static void ClearBinds();
    static void Do();

};