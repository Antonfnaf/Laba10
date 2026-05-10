#pragma once
#include <iostream>
#include "Core/enums.h"
//#include "UI/windows/IMainWindow.h"

class GameObject;

//игровое поле. хранит обьекты в ячейках на их координатах. является связкой всех обьектов
//class Field : InWindow{
//	GameObject*** objects;
//public:
//    Field(int Width, int Height);
//    
//
//
//        // Inherited via IMainWindow
//    std::vector<std::vector<Pixel>> GetInstructionLO(int width, int height) const override;
//    int GetWidth() const override;
//    int GetHeight() const override;
//};