#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include "Core/enums.h"
//#include "Core/Pixel.h"



class Bind {
private:
    std::map<KeyCode, std::function<void()>> actions;

public:
    bool contains(KeyCode key) const {
        return actions.find(key) != actions.end();
    }
    std::map<KeyCode, std::function<void()>> GetCopy() const {
        return actions;
    }
    void Add(KeyCode key, std::function<void()> action) {
        actions[key] = action;
        return;
    }
    void Add(std::map<KeyCode, std::function<void()>> localActions) {
        for (const auto& [key, action] : localActions) {
            actions.insert_or_assign(key, action);
        }
        return;
    }
    void Clear() {
        actions.clear();
    }
    std::function<void()>& operator[](KeyCode key) {
        return actions[key];
    }
};