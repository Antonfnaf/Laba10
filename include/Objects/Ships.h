#pragma once
#include <iostream>
#include <optional>
#include "enums.h"
#include "Base.h"

class Ship : GameObject{
    Ship(std::string name,
        Country country = Country::USA,
        ShipType type = ShipType::lodka,
        int volume = 1000,
        int speed = 25 
    );

    int countOfTravel, createYear, volume, speed;
    std::string name;
    ShipType type;
    Country country;
    ShipStatus status;

public:    

    std::string get_name() { return name; }
    ShipType get_type() { return type; }
    Country get_country() { return country; }
    int get_countOfTravel() { return countOfTravel; }
    int get_createYear() { return createYear; }
    int get_volume() { return volume; }
    int get_speed() { return speed; }

    void Travel();
    void Fixing(std::optional<ShipType> newType = std::nullopt,
        std::optional<int> newVolume = std::nullopt,
        std::optional<int> newSpeed = std::nullopt
        );

};
