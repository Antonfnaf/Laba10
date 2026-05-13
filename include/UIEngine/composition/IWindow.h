#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/IRenderable.h"
#include "UIEngine/input/Bind.h"

class IWindow : public IRenderable {
protected:
	Bind binds;
public:

	std::map<KeyCode, std::function<void()>> GetBinds() {
		return binds.GetCopy();
	}
};


class NullWindow : public IWindow
{
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));
	}
};
