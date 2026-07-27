#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/IRenderable.h"
#include "UIEngine/input/Bind.h"

//Интерфейс для всех обьектов 
class IWindow : public IRenderable {
protected:
	
	//Список действий с назначенными клавишами для этого обьекта
	Bind binds;
public:
	using IRenderable::GetFrame;
	//Геттер для биндов
	std::map<KeyChord, std::function<void()>> GetBinds() {
		return binds.GetCopy();
	}

	//-- from IRenderable --
	// virtual GetFrameLO(int width, int height)
	// GetFrame(int width, int height)
};

//Пустое окно. Дефолтное окно
class NullWindow : public IWindow
{
	//-- from IWindow --
	// Bind binds
	// GetFrame(int width, int height)
	// GetBinds()
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));
	}
};
