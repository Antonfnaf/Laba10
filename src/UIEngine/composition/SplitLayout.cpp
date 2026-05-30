#include "UIEngine/composition/SplitLayout.h"


std::vector<std::vector<Pixel>> SplitLayout::GetFrameLO(int width, int height, IWindow* active) const {
	std::vector<std::vector<Pixel>> result;
	if (vertical) {
		int heightFirst = static_cast<int>(std::round(height * firstRatio));
		int heightSecond = height - heightFirst;

		result = FrameTools::ConcatFrame(first->GetFrame(width, heightFirst, active), second->GetFrame(width, heightSecond, active), false);
	} else {
		int widthFirst = static_cast<int>(std::round(width * firstRatio));
		int widthSecond = width - widthFirst;

		result = FrameTools::ConcatFrame(first->GetFrame(widthFirst, height, active), second->GetFrame(widthSecond, height, active), true);
	}
	return result;
}

std::vector<ILayout*> SplitLayout::GetPaneList() {
	std::vector<ILayout*> FList = first->GetPaneList();
	std::vector<ILayout*> SList = second->GetPaneList();
	FList.insert(FList.end(), SList.begin(), SList.end());
	return FList;
}
std::vector<ILayout*> SplitLayout::GetPath(IWindow* window) {
	if (this == window) return { this };
	std::vector<ILayout*> Ans = first->GetPath(window);
	if (Ans.empty())
		Ans = second->GetPath(window);
	if (!Ans.empty())
		Ans.push_back(this);
	return Ans;
}

//bool SplitLayout::resetRatioForOnePixel(int _size, bool _first, bool _positive, int _minSize) {
//	int p = _positive ? 1 : -1;
//	int f = _first ? p : p * -1;
//	int newFirstSize = static_cast<int>(std::round(_size * firstRatio)) + f;
//	float newRatio = static_cast<float>(newFirstSize) / (_size + p);
//
//	if (newFirstSize >= _minSize && ((_size + p) - newFirstSize) >= _minSize) {
//		SetRatio(newRatio);
//		return true;
//	} else {
//		return false;
//	}
//}
//bool SplitLayout::resizeChild(SplitLayout* _first, SplitLayout* _second, int _width, int _height, bool pos, Direction dir) {
//	bool ans = false;
//	bool _isF = dir == Direction::Up || dir == Direction::Left;
//	bool _isW = dir == Direction::Right || dir == Direction::Left;
//	SplitLayout* f = _isF ? _first : _second;
//	SplitLayout* s = !_isF ? _first : _second;
//	int size = _isW ? _width : _height;
//	int spec = _isF ? size - static_cast<int>(std::round(size * firstRatio)) : size * firstRatio;
//	int size1 = _isW ? spec : _width;
//	int size2 = _isW ? _height : spec;
//	if (f) {
//		ans = f->ResizeOnePixel(size1, size2, dir, pos);
//	} else {
//		//
//		ans = resetRatioForOnePixel(size, _isF, pos, MINSIZE);
//	}
//	if (!ans) {
//		if (s) {
//			ans = s->ResizeOnePixel(size1, size2, dir, pos);
//		} else {
//			//
//			ans = resetRatioForOnePixel(size, !_isF, pos, MINSIZE);
//		}
//	}
//	return ans;
//}
//
//bool SplitLayout::ResizeOnePixel(int width, int height, Direction dir, bool pos) {
//	
//	
//	//bool ans = false;
//	//if (dir == Direction::Down || dir == Direction::Up) {
//	//	if (!pos && (vertical && height < 5)) return false;
//	//	if (!pos && ((!vertical) && height < 3)) return false;
//	//} else {
//	//	if (!pos && (vertical && width < 3)) return false;
//	//	if (!pos && ((!vertical) && width < 5)) return false;
//	//}
//
//	//SplitLayout* f = dynamic_cast<SplitLayout*>(first.get());
//	//SplitLayout* s = dynamic_cast<SplitLayout*>(second.get());
//
//
//
//	//if (!vertical && (dir == Direction::Down || dir == Direction::Up)) {
//
//	//	if (s) {
//	//		ans = s->ResizeOnePixel(width * firstRatio, height, dir, pos);
//	//	} else ans = true;
//	//	if (f && ans) {
//	//		ans = f->ResizeOnePixel(width - static_cast<int>(std::round(width * firstRatio)), height, dir, pos);
//	//		if (!ans && s)  s->ResizeOnePixel(width * firstRatio, height, dir, !pos);
//	//	} else ans = true;
//	//	return ans;
//	//} else if (vertical && (dir == Direction::Left || dir == Direction::Right)) {
//	//	if (s) {
//	//		ans = s->ResizeOnePixel(width, height * firstRatio, dir, pos);
//	//	} else ans = true;
//	//	if (f && ans) {
//	//		ans = f->ResizeOnePixel(width, height - static_cast<int>(std::round(height * firstRatio)), dir, pos);
//	//		if (!ans && s)  s->ResizeOnePixel(width, height * firstRatio, dir, !pos);
//	//	} else ans = true;
//
//	//	return ans;
//	//}
//	///////////
//
//	//return resizeChild(f, s, width, height, pos, dir);
//
//}