#include "UIEngine/composition/SplitLayout.h"

#include <cmath>


std::vector<std::vector<Pixel>> SplitLayout::GetFrameLO(int width, int height, IWindow* active) const {
	std::vector<std::vector<Pixel>> result;
	if (vertical) {
		int heightFirst = static_cast<int>(std::round(height * firstRatio));
		int heightSecond = height - heightFirst;

		result = FrameTools::ConcatFrame(first->GetFrame(width, heightFirst, active), second->GetFrame(width, heightSecond, active), false);
	} else {
          int widthFirst = static_cast<int>(std::round(width * firstRatio));
          int widthSecond = width - widthFirst;

          result = FrameTools::ConcatFrame(
              first->GetFrame(widthFirst, height, active),
              second->GetFrame(widthSecond, height, active), true);
	}
	return result;
}

std::vector<ILayout*> SplitLayout::GetPaneList() const {
	std::vector<ILayout*> FList = first->GetPaneList();
	std::vector<ILayout*> SList = second->GetPaneList();
	FList.insert(FList.end(), SList.begin(), SList.end());
	return FList;
}
std::vector<ILayout*> SplitLayout::GetPath(IWindow* window) const {
	if (this == window) return { (ILayout*)this };
	std::vector<ILayout*> Ans = first->GetPath(window);
	if (Ans.empty())
		Ans = second->GetPath(window);
	if (!Ans.empty())
		Ans.push_back((ILayout*)this);
	return Ans;
}

