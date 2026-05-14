#pragma once
#include <iostream>
#include <time.h>

#include "UIEngine/composition/IWindow.h"
#include "UI/moduls/ModuleBox.h"
#include "UI/Actions/Button.h"


class WButtonList : public IWindow
{
	std::vector<Pixel> head;
	std::vector<std::vector<Pixel>> GetFrameLO(int width,int height) override;
	std::vector<Button> button;
	int selectIndex = 0;
};

std::vector<std::vector<Pixel>> WSimpleList::GetFrameLO(int width, int height){
	std::vector<std::vector<Pixel>> pic;
	std::vector<std::vector<Pixel>> tmp;
	pic.push_back(head);

	for (int i = 0; i < modules.size(); i++) {
		tmp = button[i];
		if (selectIndex==i) {
			//todo
		}
	}
};