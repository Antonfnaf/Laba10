#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
/*
#include <conio.h>*/


#include "Core/enums.h"
#include "UIEngine/input//Bind.h"
//#include "Core/Pixel.h"

struct Action
{
    std::vector<KeyCode> keys;
    std::function<void()> action;
	operator std::function<void()>() const { return action; }
};

struct InputManager
{
    
private:
    static Bind actions;
    static KeyCode getKeyCode();
    //static std::vector<KeyCode> getKeyCodes();

public:

    static void Bind(KeyCode key, std::function<void()> action);
    static void Bind(std::map < KeyCode, std::function<void()>> localActions);
    static void ClearBinds();
    static void Do();

};

struct InputManager1
{
    
private:
    static Bind actions;
    static KeyCode getKeyCode();
    //static std::vector<KeyCode> getKeyCodes();

public:

    static void Bind(KeyCode key, std::function<void()> action);
    static void Bind(std::map < KeyCode, std::function<void()>> localActions);
    static void ClearBinds();
    static void Do();

};


