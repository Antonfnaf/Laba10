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

SplitLayout* ScreenLayout::FindSplitByDirection(const std::vector<ILayout*>& path, Direction dir) const {
	
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

void ScreenLayout::SetSplitRatio(SplitLayout* split, float newRatio) {
	if (!split) return;
	split->SetRatio(newRatio);
}

void ScreenLayout::ChangeSplitRatio(SplitLayout* split, float delta) {
	SetSplitRatio(split,split->GetRatio() + delta);
}

void ScreenLayout::ChangeSplitByPixels(SplitLayout* split, int delta) {
	std::pair<int, int> size = FindLayoutSize(split);
	int totalSize;
	if (split->IsVert()) {
		totalSize = size.second;
	} else {
		totalSize = size.first;
	}
	float deltaRatio = static_cast<float>(delta) / static_cast<float>(totalSize);
	ChangeSplitRatio(split, deltaRatio);
}


bool ScreenLayout::ResizeActiveByPixels(Direction direction, int delta) {
	// 1. Находим путь к активному окну
	if (!root.get()) return false;
	std::vector<ILayout*> path = root->GetPath(active);
	if (path.size() <2) return false;

	// 2. Находим сплит, который нужно изменять
	SplitLayout* split = FindSplitByDirection(path, direction);
	if (!split) return false;


	// 3. Определяем, первое ли окно в этом сплите
	bool isFirst = direction == Direction::Down || direction == Direction::Right;
	int finaleDelta = isFirst ? delta : delta * -1;
	ChangeSplitByPixels(split, finaleDelta);
	return true;
}


//void ScreenLayout::SwapLayouts(ILayout* a, ILayout* b) {
//
//} 

void ScreenLayout::UpdateBinds() {
	binds.ClearAll();
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
					if (!ResizeActiveByPixels(Direction::Right,1)) {
						ResizeActiveByPixels(Direction::Left, -1);
					}
				}},
				{KeyCode::ShiftLeftArrow, [&]() {
					if (!ResizeActiveByPixels(Direction::Left,1)) {
						ResizeActiveByPixels(Direction::Right, -1);
					}
				}},
				{KeyCode::ShiftDownArrow, [&]() {
					if (!ResizeActiveByPixels(Direction::Down, 1)) {
						ResizeActiveByPixels(Direction::Up, -1);
					}
				}},
				{KeyCode::ShiftUpArrow, [&]() {
					if (!ResizeActiveByPixels(Direction::Up, 1)) {
						ResizeActiveByPixels(Direction::Down, -1);
					}
				}},
			}
		);
	binds.Add(userBinds);
}