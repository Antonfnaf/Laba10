#pragma once

#include "UIEngine/composition/IWindow.h"

class IModule : public IWindow {
	int perfectWidth;
	int perfectHeight;
	

	std::vector<std::vector<Pixel>> GetFrameLO() {
		return GetFrame(perfectWidth, perfectHeight);
	}
public:

	std::vector<std::vector<Pixel>> GetFrame() const {
		return std::vector<std::vector<Pixel>> GetFrame(perfectWidth, perfectHeight);
	}

};