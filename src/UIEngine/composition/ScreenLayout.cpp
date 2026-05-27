#include "UIEngine/composition/ScreenLayout.h"



void ScreenLayout::SetFocusPane(ILayout* pane) {
	active = pane;
}
void ScreenLayout::ChangeFocus(int num) {
	if (!root.get()) return;
	std::vector<ILayout*> paneList = root->GetPaneList();
	if (paneList.empty()) return;
	int index = 0;
	for (int i = 0; i < paneList.size();i++) 
		if (paneList[i] == active)
			index = i;
	
	int newIndex = (index + num) % (int)paneList.size();
	if (newIndex < 0) newIndex += paneList.size();
	SetFocusPane(paneList[newIndex]);
}
void ScreenLayout::FocusNext() {
	ChangeFocus(1);
}
void ScreenLayout::FocusPrev() {
	ChangeFocus(-1);
}

void ScreenLayout::SetActive(IWindow* window) {
	if (!root.get()) {
		root = std::make_unique<PaneLayout>(window);
		SetFocusPane(root.get());
		return;
	}
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.empty()) return;

	PaneLayout* pane = dynamic_cast<PaneLayout*>(path[0]);
	if (pane) {
		pane->SetWindow(window);
		SetFocusPane(pane);
	}
}

void ScreenLayout::SplitActive(IWindow* window, bool vertical) {
	if (!root.get()) { 
		root = std::make_unique<PaneLayout>(window);
		SetFocusPane(root.get());
		return;
	}
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.empty()) return;

	if (path.size() == 1) {
		std::unique_ptr<ILayout> first = std::move(root);
		std::unique_ptr<ILayout> second = std::make_unique<PaneLayout>(std::move(window));
		SetFocusPane(second.get());
		root = std::make_unique<SplitLayout>(std::move(first),std::move(second),0.5,vertical);
	} else {
		SplitLayout* parent = dynamic_cast<SplitLayout*>(path[1]);
		if (!parent) return;
		if (parent->GetFirst() == path[0]) {
			std::unique_ptr<ILayout> first = std::move(parent->ReleaseFirst());
			std::unique_ptr<ILayout> second = std::make_unique<PaneLayout>(std::move(window));
			SetFocusPane(second.get());
			parent->SetFirst(std::make_unique<SplitLayout>(std::move(first), std::move(second), 0.5, vertical));
		} else {
			std::unique_ptr<ILayout> first = std::move(parent->ReleaseSecond());
			std::unique_ptr<ILayout> second = std::make_unique<PaneLayout>(std::move(window));
			SetFocusPane(second.get());
			parent->SetSecond(std::make_unique<SplitLayout>(std::move(first), std::move(second), 0.5, vertical));
		}
	}
}

void ScreenLayout::DeletePane(IWindow* pane) {
	if (!root.get()) return;
	std::vector<ILayout*> path = root->GetPath(pane);
	if (path.empty()) return;

	if (path.size() == 1) {
		SetFocusPane(nullptr);
		root = nullptr;
	} else if (path.size() == 2) {
		SplitLayout* parent = dynamic_cast<SplitLayout*>(path[1]);
		if (!parent) return;
		if (parent->GetFirst() == path[0]) {
			SetFocusPane((!parent->GetSecond()->GetPaneList().empty()) ? parent->GetSecond()->GetPaneList()[0] : nullptr);
			root = std::move(parent->ReleaseSecond());
		} else {
			SetFocusPane((!parent->GetFirst()->GetPaneList().empty()) ? parent->GetFirst()->GetPaneList()[0] : nullptr);
			root = std::move(parent->ReleaseFirst());
		}
	} else {
		SplitLayout* parent = dynamic_cast<SplitLayout*>(path[1]);
		SplitLayout* grandParent = dynamic_cast<SplitLayout*>(path[2]);
		if (!parent) return;
		if (grandParent->GetFirst() == path[1]) {
			if (parent->GetFirst() == path[0]) {
				SetFocusPane((!parent->GetSecond()->GetPaneList().empty()) ? parent->GetSecond()->GetPaneList()[0] : nullptr);
				grandParent->SetFirst(std::move(parent->ReleaseSecond()));
			} else {
				SetFocusPane((!parent->GetFirst()->GetPaneList().empty()) ? parent->GetFirst()->GetPaneList()[0] : nullptr);
				grandParent->SetFirst(std::move(parent->ReleaseFirst()));
			}
		} else {
			if (parent->GetFirst() == path[0]) {
				SetFocusPane((!parent->GetSecond()->GetPaneList().empty()) ? parent->GetSecond()->GetPaneList()[0] : nullptr);
				grandParent->SetSecond(std::move(parent->ReleaseSecond()));
			} else {
				SetFocusPane((!parent->GetFirst()->GetPaneList().empty()) ? parent->GetFirst()->GetPaneList()[0] : nullptr);
				grandParent->SetSecond(std::move(parent->ReleaseFirst()));
			}
		}
	}
}

void ScreenLayout::DeleteActive() {
	DeletePane(active);
}

SplitLayout* ScreenLayout::FindRootSplit(const std::vector<ILayout*>& path, Direction dir) const {
	
	for (int i = 1; i < path.size(); i++) {
		SplitLayout* split = dynamic_cast<SplitLayout*>(path[i]);
		if (!split) continue;

		bool vertical = split->IsVert();
		bool activeIsFirst = (split->GetFirst() == path[i - 1]);

		if (dir == Direction::Up) {
			if (vertical && !activeIsFirst) return split;
		} else if (dir == Direction::Down) {
			if (vertical && activeIsFirst) return split;
		} else if (dir == Direction::Left) {
			if (!vertical && !activeIsFirst) return split;
		} else if (dir == Direction::Right) {
			if (!vertical && activeIsFirst) return split;
		}
	}
	return nullptr;
}

std::pair<int, int> ScreenLayout::FindLayoutSize(ILayout* layoutToFind, ILayout* rootLayout, int rootWidth, int rootHeight) {
	if (!rootLayout) return std::make_pair(rootWidth, rootHeight);
	std::vector<ILayout*> path = rootLayout->GetPath(layoutToFind);
	if (path.empty()) return std::make_pair(rootWidth, rootHeight);

	for (int i = path.size() - 1; i > 0; i--) {
		SplitLayout* split = dynamic_cast<SplitLayout*>(path[i]);
		if (!split) continue;
		if (split->IsVert()) {
			if (path[i - 1] == split->GetFirst())
				rootHeight = static_cast<int>(rootHeight * split->GetRatio());
			else
				rootHeight = rootHeight - static_cast<int>(rootHeight * split->GetRatio());
		} else {
			if (path[i - 1] == split->GetFirst())
				rootWidth = static_cast<int>(rootWidth * split->GetRatio());
			else
				rootWidth = rootWidth - static_cast<int>(rootWidth * split->GetRatio());

		}
	}
	return std::make_pair(rootWidth, rootHeight);
}

std::pair<int, int> ScreenLayout::FindLayoutSize(ILayout* layout) const {
	return FindLayoutSize(layout, root.get(), screenWidth,screenHeight);
}

void ScreenLayout::ResizeActiveRatio(float newRatio, Direction direction) {
	if (!root.get()) return;
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.empty()) return;
	if (path.size() == 1) return;

	SplitLayout* resizeSplit = FindRootSplit(path, direction);
	if (!resizeSplit) return;
	resizeSplit->SetRatio(newRatio);	
}

void ScreenLayout::ChangeActiveRatio(float ratioplus, Direction direction) {
	if (!root.get()) return;
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.empty()) return;
	if (path.size() == 1) return;

	SplitLayout* resizeSplit = FindRootSplit(path, direction);
	if (!resizeSplit) return;
	if (direction == Direction::Left || direction == Direction::Up)
		resizeSplit->SetRatio(resizeSplit->GetRatio() - ratioplus);
	else
		resizeSplit->SetRatio(resizeSplit->GetRatio() + ratioplus);
}

bool ScreenLayout::ResizeActiveToOnePixel(Direction direction, bool pos) {
	if (!root.get()) return false;
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.empty()) return false;
	if (path.size() == 1) return false;


	SplitLayout* resizeSplit = FindRootSplit(path, direction);
	if (!resizeSplit) return false;
	bool activeInFirst = direction == Direction::Down || direction == Direction::Right;
	SplitLayout* Split1 = activeInFirst ? dynamic_cast<SplitLayout*>(resizeSplit->GetFirst()): dynamic_cast<SplitLayout*>(resizeSplit->GetSecond());
	SplitLayout* Split2 = !activeInFirst ? dynamic_cast<SplitLayout*>(resizeSplit->GetFirst()): dynamic_cast<SplitLayout*>(resizeSplit->GetSecond());
	
	std::pair size = FindLayoutSize(resizeSplit);
	int ratioSize = resizeSplit->IsVert() ? size.second : size.first;/*
	static_cast<int>(ratioSize * resizeSplit->GetRatio())*/
	int fSize = activeInFirst ? static_cast<int>(ratioSize * resizeSplit->GetRatio()) : ratioSize - static_cast<int>(ratioSize * resizeSplit->GetRatio());
	int sSize = !activeInFirst ? static_cast<int>(ratioSize * resizeSplit->GetRatio()) : ratioSize - static_cast<int>(ratioSize * resizeSplit->GetRatio());
	bool ans = false;
	if (resizeSplit->IsVert()) {
		if (Split1) { ans = Split1->ResizeOnePixel(size.first, fSize, direction, pos); } else { ans = true; }
		if (ans && Split2) { ans = Split2->ResizeOnePixel(size.first, sSize, (Direction)(((int)direction + 2) % 4), !pos);}
		if(!ans&&Split1){ Split1->ResizeOnePixel(size.first, fSize, direction, !pos); }
	} else {
		if (Split1) { ans = Split1->ResizeOnePixel(fSize, size.second, direction, pos); } else { ans = true; }
		if (ans && Split2) { Split2->ResizeOnePixel(sSize, size.second, (Direction)(((int)direction + 2) % 4), !pos); }
		if(!ans && Split1){ Split1->ResizeOnePixel(fSize, size.second, direction, !pos); }
	}


	int _minSize = MINSIZE;
	int inc = pos ? 1 : -1;
	float newRatio = 0;
	float oldRatio = resizeSplit->GetRatio();
	int oldSize = fSize;
	if (activeInFirst) {/*
		int i = 0;
		do {*/
		fSize = fSize + inc;
		newRatio = static_cast<float>(fSize) / ratioSize;/*
		i++;
		} while (static_cast<int>(std::round(ratioSize * newRatio)) == oldSize && i < 3);*/
		if (newRatio == oldRatio) {
			fSize = fSize + inc;
			newRatio = static_cast<float>(fSize) / ratioSize;
		}
	} else {/*
		int i = 0;
		do {*/
			fSize = fSize + inc;
			newRatio = static_cast<float>(ratioSize - fSize) / ratioSize;/*
			i++;
		} while (static_cast<int>(std::round(ratioSize * newRatio)) == (ratioSize - oldSize) && i < 3);*/
			if (newRatio == oldRatio) {
				fSize = fSize + inc;
				newRatio = static_cast<float>(ratioSize - fSize) / ratioSize;
			}
	}
	if (fSize >= _minSize && (ratioSize - fSize) >= _minSize && ans) {
		resizeSplit->SetRatio(newRatio);
		ans = true;
	} else ans = false;
	return ans;
}


//void ScreenLayout::SwapLayouts(ILayout* a, ILayout* b) {
//
//} 

void ScreenLayout::UpdateBinds() {
	binds.Clear();
	if (root.get())
		binds.Add(root->GetBinds(active));
	if(defaultBindsIsOn)
		binds.Add(
			{
				{KeyCode::CtrlL, [&]() {FocusNext(); }},
				{KeyCode::AltRightArrow, [&]() {FocusNext(); }},
				{KeyCode::CtrlH, [&]() {FocusPrev(); }},
				{KeyCode::AltLeftArrow, [&]() {FocusPrev(); }},
				{KeyCode::CtrlRightArrow, [&]() {SplitActive(new NullWindow(),false); }},
				{KeyCode::CtrlDownArrow, [&]() {SplitActive(new NullWindow(),true); }},
				{KeyCode::CtrlD, [&]() {DeleteActive(); }},
				{KeyCode::ShiftRightArrow, [&]() {
					if (!ResizeActiveToOnePixel(Direction::Right,true)) {
						ResizeActiveToOnePixel(Direction::Left, false);
					} 
				}},
				{KeyCode::ShiftLeftArrow, [&]() {
					if (!ResizeActiveToOnePixel(Direction::Left,true)) {
						ResizeActiveToOnePixel(Direction::Right, false);
					} 
				}},
				{KeyCode::ShiftDownArrow, [&]() {
					if (!ResizeActiveToOnePixel(Direction::Down,true)) {
						ResizeActiveToOnePixel(Direction::Up, false);
					} 
				}},
				{KeyCode::ShiftUpArrow, [&]() {
					if (!ResizeActiveToOnePixel(Direction::Up,true)) {
						ResizeActiveToOnePixel(Direction::Down, false);
					} 
				}},
			}
		);
	binds.Add(userBinds);
}