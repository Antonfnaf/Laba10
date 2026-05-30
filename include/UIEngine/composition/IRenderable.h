#pragma once
#include <vector>
#include "Core/Pixel.h"


class IRenderable
{
protected:
	//Возвращает картинку обьекта под заданные размеры. Логика может меняться от класса.
	virtual std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const = 0;
public:
	//Возвращает прямоугольную картинку пикселей по данным размерам. Не для реализации, использует GetFrameLO
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
};
