#include "UIEngine/rendering/Renderer.h"



std::vector<std::vector<Pixel>> Renderer::prevCadre = std::vector<std::vector<Pixel>>();



void Renderer::UpdateCadre(std::vector<std::vector<Pixel>> newCadre) {
	int ConsoleWidth = ConsoleTools::GetConsoleWidth();
	int ConsoleHeight = ConsoleTools::GetConsoleHeight();

	bool NeedMove = false;

	std::cout << "\033[0;0H";

	int rowsToDraw = (int)newCadre.size() < ConsoleHeight ? (int)newCadre.size() : ConsoleHeight;

	for (int i = 0; i < rowsToDraw; i++){
		
		if (i>0 && !NeedMove)
			std::cout << "\n";

		int colsToDraw = (int)newCadre[i].size() < ConsoleWidth ? (int)newCadre[i].size() : ConsoleWidth;

		for (int j = 0; j < colsToDraw; j++){
			bool prevYIs = /*prevCadre.size() > i && */newCadre.size() /*>=*/== prevCadre.size();

			if (prevYIs) {
			bool prevXIs = /*prevCadre[i].size() > j && */newCadre[i].size() /*>=*/== prevCadre[i].size();
				if ((prevXIs && prevCadre[i][j] == newCadre[i][j])) {
					//std::cout << "\033[C";
					NeedMove = true;
					continue;
				}
			}
			if (NeedMove) {
				std::cout << "\033["<<i+1<<";"<<j+1<<"H"<< newCadre[i][j].Get();
				NeedMove = false;
			}
			else {
			std::cout << newCadre[i][j].Get();
			}
		}
	}
	std::cout << std::flush;
	prevCadre = std::move(newCadre);
}