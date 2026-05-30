#pragma once
#include <string>
#include <functional>
#include <map>

#include "Core/enums.h"
//#include "Core/Pixel.h"



class Bind {
private:
    std::map<KeyCode, std::function<void()>> actions;

public:
    std::function<void()>& operator[](KeyCode key) {
        return actions[key];
    }
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
    void Add(Bind bind) {
        Add(bind.GetCopy());
        return;
    }
    void Add(std::map<KeyCode, std::function<void()>> localActions) {
        for (const auto& [key, action] : localActions) 
            actions.insert_or_assign(key, action);
        return;
    }
    void ClearAll() {
        actions.clear();
    }
    void ClearKey(KeyCode key) {
        actions[key] = [](){};
    }
};