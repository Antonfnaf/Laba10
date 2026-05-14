#pragma once
#include <string>
#include <functional>
#include <vector>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "Core/Utils.h"


class Button
{
	std::functional<void()> action;
	std::string name;

public:
	Button() : action([](){}), name(NONAME) {}
	Button(std::string name, std::functional<void()> action) : action(action), name(name) {}

	void operator()() const { return action(); }
	operator std::string() const { return name; }
	operator std::vector<std::vector<Pixel>> const { return PixelParser::Parse(name); }
};



















//
//class Button {
//	Button(std::string name, Color fg = Color::white, Color bg = Color::black, std::function<bool()> act) : name(name), bg(bg), fg(fg), action(act) {}
//	std::function<ActionResult()> action;
//	std::string name;
//	Color bg;
//	Color fg;
//	bool selected;
//
//public:
//	
//	GetVector() {
//		std::vector<Pixel> button;
//		if (selected)
//			for (char c : name)
//				button.push_back(Pixel(c, bg, fg));
//		else
//			for (char c : name)
//				button.push_back(Pixel(c, fg, bg));
//		return button;
//	}
//};
//
//std::vector<Pixel> operator+(std::vector<Pixel> vec, const Button& button) {
//	vec.insert(vec.end(),button.GetVector())
//}