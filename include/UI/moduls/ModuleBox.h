#pragma once
#include "UI/moduls/IModule.h"

class ModuleBox : public IModule {
private:
	IModule* module;

	MPlace place = MPlace::Free;
	int positionX = 0;
	int positionY = 0;

	bool interactive;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override  {
		return module->GetFrame(width, height);
	}
	
public:
	ModuleBox(IModule* module, MPlace place, bool interactive = false): module(module), place(place), interactive(interactive) {}
	ModuleBox(IModule* module, int X, int Y, bool interactive = false): module(module), place(MPlace::Free), positionX(X), positionY(Y), interactive(interactive) {}
	void SetPlace(MPlace newPlace) { place = newPlace;}
	MPlace GetPlace() { return place; }

};