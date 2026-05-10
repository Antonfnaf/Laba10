#pragma once
#include <iostream>
#include <string>

#include "Core/enums.h"



struct Pixel {
	Pixel(std::string symbol = " ", Color fg = Color::White, Color bg = Color::Black) : symbol(symbol), bgColor(bg), fgColor(fg) {}
	std::string symbol;//символ для отображения в консоли
	Color bgColor;//цвет фона для отображения в консоли
	Color fgColor;//цвет символа для отображения в консоли

	bool operator==(const Pixel& other) const {
		return symbol == other.symbol && fgColor == other.fgColor && bgColor == other.bgColor;
	}
	std::string Get() {
		return "\033[38;05;" + std::to_string((int)fgColor) + ";48;05;" + std::to_string((int)bgColor) + "m" + symbol;
	}
};