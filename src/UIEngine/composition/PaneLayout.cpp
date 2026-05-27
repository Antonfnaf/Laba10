#include "UIEngine/composition/PaneLayout.h"



std::vector<std::vector<Pixel>> PaneLayout::GetFrameLO(int width, int height) const {
	return window->GetFrame(width, height);
}
std::vector<std::vector<Pixel>> PaneLayout::GetFrameLO(int width, int height, IWindow* active) const {//!!!!!!!!!!!!!!!!!!!!!!!
	Color borderColor = Compare(active) ? Color::White7 : Color::Gray3;
	OldFrame frame = OldFrame(BorderType::Curcle, borderColor);//!!!!!!!!!!!!!!!!!!!!!!!!!
	if (width < 2 || height < 2) {
		return {};
	}
	return frame.PasteInFrame(window->GetFrame(width - 2, height - 2));
}

std::vector<ILayout*> PaneLayout::GetPaneList() {
	return { this };
}
std::vector<ILayout*> PaneLayout::GetPath(IWindow* window) {
	if (Compare(window)) {
		return { this };
	}
	if (this == window) {
		return { this };
	}
	return {};
}