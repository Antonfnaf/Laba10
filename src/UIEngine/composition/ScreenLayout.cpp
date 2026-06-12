#include "UIEngine/composition/ScreenLayout.h"

Bind ScreenLayout::defaultBinds;


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

Place ScreenLayout::FindLayoutPlace(ILayout* layoutToFind, ILayout* rootLayout, int rootWidth, int rootHeight)  {
	if (!rootLayout) return Place(0,0,rootWidth, rootHeight);
	std::vector<ILayout*> path = rootLayout->GetPath(layoutToFind);
	if (path.empty()) return Place(0, 0, rootWidth, rootHeight);

	int x = 0, y = 0;

	for (int i = path.size() - 1; i > 0; i--) {
		SplitLayout* split = dynamic_cast<SplitLayout*>(path[i]);
		if (!split) continue;
		if (split->IsVert()) {
			if (path[i - 1] == split->GetFirst())
				rootHeight = static_cast<int>(rootHeight * split->GetRatio());
			else {
				y+= static_cast<int>(rootHeight * split->GetRatio());
				rootHeight = rootHeight - static_cast<int>(rootHeight * split->GetRatio());
			}
		} else {
			if (path[i - 1] == split->GetFirst())
				rootWidth = static_cast<int>(rootWidth * split->GetRatio());
			else {
				x+= static_cast<int>(rootWidth * split->GetRatio());
				rootWidth = rootWidth - static_cast<int>(rootWidth * split->GetRatio());
			}
		}
	}
	return Place(x, y, rootWidth, rootHeight);
}

Place ScreenLayout::FindLayoutPlace(ILayout* layout) const {
	return FindLayoutPlace(layout, root.get(), screenWidth,screenHeight);
}

ILayout* ScreenLayout::FindLayoutByDirection(ILayout* layoutFrom,Direction dir) const {
	bool searchVert = dir == Direction::Up || dir == Direction::Down;
	bool searchFirst = dir == Direction::Down || dir == Direction::Right;
	SplitLayout* splitForSearch = FindSplitByDirection(GetPath(layoutFrom),dir);
	if (!splitForSearch) return nullptr;
	Place searchSplitPlace = FindLayoutPlace(splitForSearch);
	Place activePlace = FindLayoutPlace(layoutFrom,splitForSearch,searchSplitPlace.w,searchSplitPlace.h);

	int centerPoint = searchVert ? activePlace.x + activePlace.w * 0.5 : activePlace.y + activePlace.h * 0.5;
	Place tempPlace = FindLayoutPlace(!searchFirst ? splitForSearch->GetFirst() : splitForSearch->GetSecond(), splitForSearch, searchSplitPlace.w, searchSplitPlace.h);;
	SplitLayout* tempSplit = dynamic_cast<SplitLayout*>(!searchFirst ? splitForSearch->GetFirst() : splitForSearch->GetSecond());
	if (!tempSplit) return !searchFirst ? splitForSearch->GetFirst() : splitForSearch->GetSecond();
	while (tempSplit) {
		SplitLayout* tSplit;
		if (tempSplit->IsVert() == searchVert) {
			tSplit = dynamic_cast<SplitLayout*>(searchFirst ? tempSplit->GetFirst() : tempSplit->GetSecond());
			if (!tSplit) return searchFirst ? tempSplit->GetFirst() : tempSplit->GetSecond();
			tempSplit = tSplit;
			continue;
		}
		Place tplace = FindLayoutPlace(tempSplit->GetFirst(),tempSplit,tempPlace.w,tempPlace.h);
		int Tcoord = searchVert ? tempPlace.x : tempPlace.y;
		int tcoord = searchVert ? tplace.x : tplace.y;
		if (Tcoord+tcoord <= centerPoint && Tcoord + tcoord + (searchVert ? tplace.w : tplace.h) > centerPoint) {
			tSplit = dynamic_cast<SplitLayout*>(tempSplit->GetFirst());
			if (!tSplit) 
				return tempSplit->GetFirst();
			tempSplit = tSplit;
			tempPlace = tplace;
			if (searchVert)
				tempPlace.x += Tcoord;
			else
				tempPlace.y += Tcoord;
			continue;
		}
		tSplit = dynamic_cast<SplitLayout*>(tempSplit->GetSecond());
		tempPlace = FindLayoutPlace(tempSplit->GetSecond(), tempSplit, tempPlace.w, tempPlace.h);
		if (!tSplit)
			return tempSplit->GetSecond();
		tempSplit = tSplit;
		if (searchVert)
			tempPlace.x += Tcoord;
		else
			tempPlace.y += Tcoord;
	}
	return nullptr;
}
ILayout* ScreenLayout::FindLayoutByDirection(Direction dir) const {
	return FindLayoutByDirection(active, dir);
}

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
void ScreenLayout::ChangeFocus(Direction dir) {
	ILayout* newActive = FindLayoutByDirection(dir);
	if (!newActive) return;
	SetFocusPane(newActive);
}

void ScreenLayout::FocusNext() {
	ChangeFocus(1);
}
void ScreenLayout::FocusPrev() {
	ChangeFocus(-1);
}

void ScreenLayout::SetContent(ILayout* target, IWindow* window) {
	if (!root.get()) {
		root = std::make_unique<PaneLayout>(window);
		SetFocusPane(root.get());
		return;
	}
	std::vector<ILayout*> path = root->GetPath(target);
	if (path.empty()) return;

	PaneLayout* pane = dynamic_cast<PaneLayout*>(path[0]);
	if (pane) {
		pane->SetWindow(window);
		SetFocusPane(pane);
	}
}
void ScreenLayout::SetActive(IWindow* window) {
	SetContent(active, window);
}

void ScreenLayout::Split(ILayout* targetLayout, IWindow* window, bool vertical) {
	if (!root.get()) { 
		root = std::make_unique<PaneLayout>(window);
		SetFocusPane(root.get());
		return;
	}
	std::vector<ILayout*> path = root->GetPath(targetLayout);
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
void ScreenLayout::SplitActive(IWindow* window, bool vertical) {
	Split(active, window, vertical);
}

void ScreenLayout::SwapLayouts(ILayout* a, ILayout* b){
	if (!a || !b) return;
	if (!root.get()) return;
	std::vector<ILayout*> A = root->GetPath(a);
	if (A.empty() || A.size()<2) return;
	for (int i = 0; i < A.size(); i++) 
		if (A[i] == b) return;
	
	std::vector<ILayout*> B = root->GetPath(b);
	if (B.empty() || B.size()<2) return;
	for (int i = 0; i < B.size(); i++)
		if (B[i] == a) return;

	std::unique_ptr<ILayout> temp;
	SplitLayout* parentA = dynamic_cast<SplitLayout*>(A[1]);
	SplitLayout* parentB = dynamic_cast<SplitLayout*>(B[1]);
	if (!parentA || !parentB) return;
	if (parentA->GetFirst() == a) {
		temp = parentA->ReleaseFirst();
		if (parentB->GetFirst()==b) {
			parentA->SetFirst( parentB->ReleaseFirst());
			parentB->SetFirst(std::move(temp));
		}else if (parentB->GetSecond() == b) {
			parentA->SetFirst( parentB->ReleaseSecond());
			parentB->SetSecond(std::move(temp));
		}
	} else if (parentA->GetSecond()==a) {
		temp = parentA->ReleaseSecond();
		if (parentB->GetFirst() == b) {
			parentA->SetSecond(parentB->ReleaseFirst());
			parentB->SetFirst(std::move(temp));
		} else if (parentB->GetSecond() == b) {
			parentA->SetSecond(parentB->ReleaseSecond());
			parentB->SetSecond(std::move(temp));
		}
	}
}
void ScreenLayout::SwapLayouts(ILayout* layoutFrom, Direction dir) {
	SwapLayouts(layoutFrom, FindLayoutByDirection(dir));
}
void ScreenLayout::SwapLayouts(Direction dir) {
	SwapLayouts(active, dir);
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


void ScreenLayout::SetSplitRatio(SplitLayout* split, float newRatio) {
	if (!split) return;
	split->SetRatio(newRatio);
}

void ScreenLayout::ChangeSplitRatio(SplitLayout* split, float delta) {
	SetSplitRatio(split,split->GetRatio() + delta);
}

void ScreenLayout::ChangeSplitByPixels(SplitLayout* split, int delta) {
	Place size = FindLayoutPlace(split);
	int totalSize;
	if (split->IsVert()) {
		totalSize = size.h;
	} else {
		totalSize = size.w;
	}
	float deltaRatio = static_cast<float>(delta) / static_cast<float>(totalSize);
	ChangeSplitRatio(split, deltaRatio);
}

bool ScreenLayout::ResizeActiveByPixels(ILayout* layoutfrom, Direction direction, int delta) {
	// 1. Находим путь к активному окну
	if (!root.get()) return false;
	std::vector<ILayout*> path = root->GetPath(layoutfrom);
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
bool ScreenLayout::ResizeActiveByPixels(Direction direction, int delta) {
	return ResizeActiveByPixels(active, direction, delta);
}



void ScreenLayout::UpdateBinds() {
	binds.ClearAll();
	if (root.get())
		binds.Add(root->GetBinds(active));
	if(defaultBindsIsOn)
		binds.Add(defaultBinds);
	binds.Add(userBinds);
}