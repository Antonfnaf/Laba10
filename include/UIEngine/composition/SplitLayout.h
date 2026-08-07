#pragma once
#include <vector>
#include <algorithm>
#include <memory>

#include "Core/Pixel.h"
#include "Core/Utils.h"
#include "UIEngine/composition/ILayout.h"


class SplitLayout final : public ILayout
{
	float firstRatio = 0.5;
	bool vertical = false;
	std::unique_ptr<ILayout> first;
	std::unique_ptr<ILayout> second;
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override;


	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return GetFrameLO(width, height, nullptr);
	}
	void UpdateBinds() override {
		binds.ClearAll();
		binds.Add(first->GetBinds());
		binds.Add(second->GetBinds());
	}
	void UpdateBinds(IWindow* active) override {
		binds.ClearAll();
		binds.Add(first->GetBinds(active));
		binds.Add(second->GetBinds(active));
	}

	//bool resetRatioForOnePixel(int _size, bool _first, bool _positive, int _minSize = MINSIZE);
	//bool resizeChild(SplitLayout* _first, SplitLayout* _second, int _width, int _height, bool pos, Direction dir);
public:
	SplitLayout(std::unique_ptr<ILayout> firstLayout, std::unique_ptr<ILayout> secondLayout, float ratio = 0.5, bool vert = false) : first(std::move(firstLayout)), second(std::move(secondLayout)), firstRatio(std::clamp(ratio, 0.0f, 1.0f)), vertical(vert) {}


	bool IsVert() { return vertical; }
	float GetRatio() { return firstRatio; }
	void SetRatio(float newRatio) { firstRatio = std::clamp(newRatio, 0.0f, 1.0f); }
	//bool ResizeOnePixel(int width, int height, Direction dir, bool pos);

	void SetFirst(std::unique_ptr<ILayout> newLayout) {
		if (newLayout != nullptr) {
			first = std::move(newLayout);
		}
	}
	void SetSecond(std::unique_ptr<ILayout> newLayout) {
		if (newLayout != nullptr) {
			second = std::move(newLayout);
		}
	}
	ILayout* GetFirst() const { return first.get(); }
	ILayout* GetSecond() const { return second.get(); }


	std::unique_ptr<ILayout> ReleaseFirst() { return std::move(first); }
	std::unique_ptr<ILayout> ReleaseSecond() { return std::move(second); }



	std::vector<ILayout*> GetPaneList() const override;
	std::vector<ILayout*> GetPath(IWindow* window) const override;




	//-- from ILayout --
	// Bind binds
	// virtual GetFrameLO(int width, int height)
	// GetFrame(int width, int height)
	// GetFrame(int width, int height, IWindow* active)
	// GetBinds();
	// GetBinds(IWindow* active);
};
	///////////////////////////////////////////////////////////////

