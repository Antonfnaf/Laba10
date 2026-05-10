#pragma once
#include <string>
#include <functional>
#include <vector>

#include "UIEngine/composition/IRenderable.h"

#include "Core/enums.h"
#include "Core/Pixel.h"



class IActionBlock : public IRenderable
{
	std::functional<void()> action;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) override {
	
	}

public:
	std::vector<std::vector<Pixel>> GetFrame() override;
	IActionBlock() : action([](){}) {}
	IActionBlock(std::functional<void()> act) : action(act) {}

	void operator()() const { return action(); }
	operator std::string() const { return name; }
};










