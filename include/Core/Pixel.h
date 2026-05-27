#pragma once
#include <iostream>
#include <string>

#include "Core/enums.h"



struct Pixel {
	Pixel(std::string symbol = " ", Color fg = Color::White, Color bg = Color::Black) : symbol(symbol), bgColor(bg), fgColor(fg) {}
	std::string symbol;//символ для отображения в консоли
	Color bgColor;//цвет фона для отображения в консоли
	Color fgColor;//цвет символа для отображения в консоли
	bool reverse = 0;
	bool bold = 0;
	bool cursive = 0;
	bool under = 0;
	bool strike = 0;

	bool operator==(const Pixel& other) const {
		return
			symbol == other.symbol &&
			fgColor == other.fgColor &&
			bgColor == other.bgColor &&
			reverse == other.reverse &&
			bold == other.bold &&
			cursive == other.cursive &&
			under == other.under &&
			strike == other.strike;
	}
	std::string Get() {
		std::string result;
		result += "\033[0;";
		if (reverse)
			result += "7;";
		if (bold)
			result += "1;";
		if (cursive)
			result += "3;";
		if (under)
			result += "4;";
		if (strike)
			result += "9;";
		result += "38;5;" + std::to_string((int)fgColor) + ";48;5;" + std::to_string((int)bgColor) + "m" + symbol;

		return result;
	}
};