#pragma once
#include <iostream>
#include <time.h>

#include "UIEngine/composition/IWindow.h"
#include "UI/moduls/ModuleBox.h"
#include "UI/Actions/Button.h"


class WButtonList : public IWindow
{
private:
	std::vector<Pixel> head;
	std::vector<Button> button;
	std::vector<std::vector<Pixel>> GetFrameLO(int width,int height) const override;

	mutable int num = 0;
	int selIndex = 0;
public:
	WButtonList(std::string name = "", std::vector<Button> button = std::vector<Button>());
	WButtonList(std::vector<Pixel> name = Converter::ParseToPic("")[0], std::vector<Button> button = std::vector<Button>());

	void AddButton(Button but) { button.push_back(but); }
	void AddButton(std::string name, std::function<void()> act) { button.push_back(Button(name,act)); }
	void SetSelect(int num) {
		int si = selIndex + num;
		int a = (si >= 0 ? si : (si) * -1) % button.size();
		selIndex = (si >=0? a : button.size() - a);

	}
	void Do() { button[selIndex](); }
};







WButtonList::WButtonList(std::string name, std::vector<Button> button) : head(Converter::ParseToPic(name)[0]), button(button) {
	binds.Add({
		{KeyChord::UpArrow, [&]()  {SetSelect(-1); }},
		{KeyChord::DownArrow, [&]() {SetSelect(1); }},
		{KeyChord::Enter, [&]() { Do(); }},
		});
}
WButtonList::WButtonList(std::vector<Pixel> name, std::vector<Button> button) : head(name), button(button) {
	binds.Add({
{KeyChord::UpArrow, [&]() { SetSelect(-1); }},
{KeyChord::DownArrow, [&]() { SetSelect(1); }},
{KeyChord::Enter, [&]() { Do(); }},
		});
}

std::vector<std::vector<Pixel>> WButtonList::GetFrameLO(int width, int height) const {
	std::vector<std::vector<Pixel>> pic;
	std::vector<std::vector<Pixel>> tmp;
	pic.push_back(head);

	for (int i = 0; i < button.size(); i++) {
		if (selIndex == i) 
			tmp.push_back(button[i].GetSelect());
		else
			tmp.push_back(button[i].Get());
	}

	if (num > selIndex) {
		num = selIndex;
	}
	if (height + num < selIndex + 2) {
		num = selIndex - height + 2;
	}

	
	for (int i = num; i < tmp.size(); i++) {
		pic.push_back(tmp[i]);
	}
	return pic;
};