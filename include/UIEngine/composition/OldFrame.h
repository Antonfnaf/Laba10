#pragma once
#include <iostream>
#include <vector>

#include "Core/enums.h"
#include "Core/Pixel.h"


class OldFrame {

	BorderType type;
	int thickness;
	Color bgColor;
	Color fgColor;
	std::string ChangeBorder(int i, int j, BorderPlace corner, BorderPlace edgeH, BorderPlace edgeV);
	std::string GetBorder(BorderPlace place) { return GetBorder(type, place); }
public:
	OldFrame(BorderType type = BorderType::Simple, Color fg = Color::White, Color bg = Color::Black, int thickness = 1);
	static std::string GetBorder(BorderType type, BorderPlace place);
	int Get_Thickness() { return thickness; }
	void SetType(BorderType type) { this->type = type; }
	void SetThickness(int thickness) { this->thickness = thickness; }
	void SetBg(Color bgColor) { this->bgColor = bgColor; }
	void SetFg(Color fgColor) { this->fgColor = fgColor; }

	std::vector<std::vector<Pixel>> const PasteInFrame(std::vector<std::vector<Pixel>> picToPaste);
};