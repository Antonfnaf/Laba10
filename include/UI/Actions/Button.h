#pragma once
#include <string>
#include <functional>
#include <vector>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "Core/Utils.h"


class Button
{
	std::function<void()> action;
	std::string name;
	std::vector<std::vector<Pixel>> vname = std::vector<std::vector<Pixel>>();
public:
	Button() : action([](){}), name("NONAME"), vname(Converter::ParseToPic(name)){}
	Button(std::string name, std::function<void()> action) : action(action), name(name), vname(Converter::ParseToPic(name)) {
		name = Converter::ParseToString(vname);
	}
	Button(std::vector<std::vector<Pixel>> name, std::function<void()> action) : action(action), name(Converter::ParseToString(name)), vname(name) {}

	void operator()() const { return action(); }
	operator std::string() const { return name; }
	operator std::vector<std::vector<Pixel>>() const { return vname; }
	std::vector<Pixel> Get() const { return vname[0]; }
	std::vector<Pixel> GetSelect() const {
		auto v = vname;
		for (int i = 0; i < v[0].size(); i++) {
		v[0][i] = Pixel(v[0][i].symbol,v[0][i].fgColor,v[0][i].bgColor);
		v[0][i].reverse = true;
		}
		return v[0]; 
	}
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