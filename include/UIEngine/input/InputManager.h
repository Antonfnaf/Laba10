#pragma once
#include <functional>
#include <map>

/*
#include <conio.h>*/


#include "Core/enums.h"
#include "UIEngine/input//Bind.h"
#include "UIEngine/input/InputHandler.h"
//#include "Core/Pixel.h"


struct InputManager
{
    
private:

    static InputHandler handler;
    static Bind actions;
    static KeyCode lastKeyCode;
    static InputEvent lastRawEvent; // Для хранения предыдущего необработанного события (для сравнения)
    static KeyEvent lastKeyEvent; // Для хранения предыдущего события клавиши (для сравнения)

    static KeyCode GetNewKeyCode();
public:
    static KeyCode GetLastKeyCode() {
        return lastKeyCode;
    }
    static InputEvent GetLastRawEvent() {
        return lastRawEvent;
    }
    static KeyEvent GetLastKeyEvent() {
        return lastKeyEvent;
    }
    /////////////
    /*inline static bool hasKey() {
        return _kbhit() != 0;
    }*/
    static void Bind(KeyCode key, std::function<void()> action);
    static void Bind(std::map < KeyCode, std::function<void()>> localActions);
    static void ClearBinds();
    static void Update() {
        lastKeyCode = GetNewKeyCode();
    }
    static void Do();

};