#pragma once
#include <iostream>

#include "Core/enums.h"

#include "UI/windows/IActionWindow.h"


class ActionWindow : public IActionWindow
{
private:

public:

};

class NullWindow : public IWindow
{
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));
	}
};