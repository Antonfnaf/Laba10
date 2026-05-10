#pragma once
#include <iostream>
#include <vector>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "Core/Utils.h"


class Renderer {
	static std::vector<std::vector<Pixel>> prevCadre;

public:
	static void UpdateCadre(std::vector<std::vector<Pixel>> newCadre);
};