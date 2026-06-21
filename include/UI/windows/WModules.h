#include "UIEngine/composition/ILayout.h"

class WModules : public IWindow
{

	std::vector<std::unique_ptr<IModule>> modules;

	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return GetFrameLO(width, height, nullptr);
	}