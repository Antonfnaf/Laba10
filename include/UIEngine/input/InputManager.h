#pragma once
#include <functional>
#include <map>

/*
#include <conio.h>*/


#include "Core/enums.h"
#include "UIEngine/input//Bind.h"
#include "UIEngine/input/InputHandler.hpp"
//#include "Core/Pixel.h"


struct InputManager
{
    
private:

    static InputHandler handler;
    static Bind actions;
    static KeyCode getKeyCode();

public:
    /////////////
    static void Init();      // ��������� ����� �����
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