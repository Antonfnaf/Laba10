#pragma once

#include "UIEngine/composition/IWindow.h"

class IModule : public IWindow
{
protected:
	int perfectWidth = 0;
	int perfectHeight = 0;



public:

	using IWindow::GetFrame;
	std::vector<std::vector<Pixel>> GetFrame() {
		return GetFrame(perfectWidth, perfectHeight);
	}

};