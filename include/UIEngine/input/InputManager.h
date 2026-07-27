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
    std::vector<KeyChord> keys;
    std::function<void()> action;
	operator std::function<void()>() const { return action; }
};

struct InputManager
{
    
private:
    static Bind actions;
    static KeyChord getKeyCode();
    //static std::vector<KeyCode> getKeyCodes();

public:

    static void Bind(KeyChord key, std::function<void()> action);
    static void Bind(std::map < KeyChord, std::function<void()>> localActions);
    static void ClearBinds();
    static void Do();

};

struct InputManager1
{
    
private:
    static Bind actions;
    static KeyChord getKeyCode();
    //static std::vector<KeyCode> getKeyCodes();

public:

    static void Bind(KeyChord key, std::function<void()> action);
    static void Bind(std::map < KeyChord, std::function<void()>> localActions);
    static void ClearBinds();
    static void Do();

};


