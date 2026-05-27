#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/ILayout.h"
#include "UIEngine/input/Bind.h"

#include "UIEngine/composition/OldFrame.h"//!!!!!!!!!!!!!!!!!!!!!!!!!!



class PaneLayout : public ILayout
{
public:
	bool const Compare(IWindow* active) const {
		return this == active;
	}
private:
	IWindow* window;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override;
	void UpdateBinds() override {
		binds.Clear();
		binds.Add(window->GetBinds());
	}

	void UpdateBinds(IWindow* active) override {
		binds.Clear();
		if (Compare(active))
			binds.Add(window->GetBinds());
	}
public:
	PaneLayout() : window(nullptr) {}
	PaneLayout(IWindow* nwindow) : window(nwindow) {}

	void SetWindow(IWindow* newWindow) {
		window = newWindow;
	}
	std::vector<ILayout*> GetPaneList() override;
	std::vector<ILayout*> GetPath(IWindow* window) override;
};
