#pragma once

#include "UIEngine/composition/IWindow.h"

class IModule : public IWindow
{
protected:
	int perfectWidth = 0;
	int perfectHeight = 0;



public:

	std::vector<std::vector<Pixel>> GetFrame(int width, int height) const {
		if (height < 0)
			height = 0;
		if (width < 0)
			width = 0;

		std::vector<std::vector<Pixel>> frame = GetFrameLO(width, height);
		if (frame.size() < height)
			frame.insert(frame.end(), height - frame.size(), std::vector<Pixel>());
		else if (frame.size() > height)
			frame.erase(frame.begin() + height, frame.end());

		for (int i = 0; i < height; i++) {
			if (frame[i].size() < width)
				frame[i].insert(frame[i].end(), width - frame[i].size(), Pixel());
			else if (frame[i].size() > width)
				frame[i].erase(frame[i].begin() + width, frame[i].end());
		}
		return frame;
	};

	std::vector<std::vector<Pixel>> GetFrame() {
		return GetFrame(perfectWidth, perfectHeight);
	}

};