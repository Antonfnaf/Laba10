#pragma once
#include <vector>
#include <math.h>
#include <utility>

#include "Core/Pixel.h"
#include "UIEngine/composition/ILayout.h"

#include "UIEngine/composition/PaneLayout.h"
#include "UIEngine/composition/SplitLayout.h"


class ScreenLayout final : public ILayout {
	std::unique_ptr<ILayout> root;
	IWindow* active;
	int screenWidth;
	int screenHeight;
	
	std::pair<int, int> FindLayoutSize(ILayout* layout);
	
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override {
		if(root.get())
			return root->GetFrame(width, height, active);
		return {};
	}
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return GetFrameLO(width, height, active);
	}

	//void SwapLayouts(ILayout* a, ILayout* b);
	void UpdateBinds();
	void UpdateBinds(IWindow* window) { UpdateBinds(); }

public:
	ScreenLayout(std::unique_ptr<ILayout> layout) : root(std::move(layout)), active((!root->GetWindows().empty()) ? root->GetWindows()[0] : nullptr) {}


	void ChangeFocus(int num);
	void FocusNext();
	void FocusPrev();
	void DeleteActive();
	static std::pair<int, int> FindLayoutSize(ILayout* layoutToFind, ILayout* rootLayout, int rootWidth,int rootHeight);
	std::map<KeyCode, std::function<void()>> GetBinds() {
		binds.Clear();
		UpdateBinds();
		return binds.GetCopy();
	}
	void SetActive(std::unique_ptr<IWindow> window);
	void SplitActive(std::unique_ptr<IWindow> window, bool vertical = false);
	void ResizeActiveRatio(float newRatio, Direction direction);
	void ChangeActiveRatio(float ratioplus, Direction direction);
	bool ResizeActiveToOnePixel(Direction direction, bool pos);

	std::vector<std::vector<Pixel>> GetScreen(int width, int height) {
		screenWidth = width;
		screenHeight = height;
		return GetFrame(width, height, active);
	};








	std::vector<IWindow*> GetWindows() const { return root->GetWindows(); }
	std::vector<ILayout*> GetPath(IWindow* window) override { return root->GetPath(window); }

};
