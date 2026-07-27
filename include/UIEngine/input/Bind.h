#pragma once
#include <string>
#include <functional>
#include <map>

#include "Core/enums.h"
//#include "Core/Pixel.h"



class Bind {
private:
    std::map<KeyChord, std::function<void()>> actions;

public:
    std::function<void()>& operator[](KeyChord key) {
        return actions[key];
    }
    bool contains(KeyChord key) const {
        return actions.find(key) != actions.end();
    }
    std::map<KeyChord, std::function<void()>> GetCopy() const {
        return actions;
    }
    void Add(KeyChord key, std::function<void()> action) {
        actions[key] = action;
        return;
    }
    void Add(Bind bind) {
        Add(bind.GetCopy());
        return;
    }
    void Add(std::map<KeyChord, std::function<void()>> localActions) {
        for (const auto& [key, action] : localActions) 
            actions.insert_or_assign(key, action);
        return;
    }
    void ClearAll() {
        actions.clear();
    }
    void ClearKey(KeyChord key) {
        actions[key] = [](){};
    }
};