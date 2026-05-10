#pragma once
#include "UI/moduls/IModule.h"


class TextBox : public IModule {
	std::vector<std::vector<Pixel>> text;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) override {
		return text;
	}
public:
	TextBox(std::vector<std::vector<Pixel>> text) : text(text)  {
		perfectHeight = text.size();
		int max = 0;
		for (int i = 0; i < perfectHeight; i++) {
			max = text[i].size() > max ? text[i].size() : max;
		}
		perfectWidth = max;
	}
	void SetText(std::vector<std::vector<Pixel>> text) { this.text = text; }
};