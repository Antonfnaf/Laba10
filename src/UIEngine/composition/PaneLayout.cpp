#include "UIEngine/composition/PaneLayout.h"

Color PaneLayout::defaultBorderBG = Color::Black;
Color PaneLayout::defaultSelectBorderBG = Color::Black;
Color PaneLayout::defaultBorderFG= Color::Gray3;
Color PaneLayout::defaultSelectBorderFG = Color::White4;
int PaneLayout::defaultBorderThickness = 1;
BorderType PaneLayout::defaultBorderType = BorderType::Simple;
BorderType PaneLayout::defaultSelectBorderType = BorderType::Heavy;

std::vector<std::vector<Pixel>> PaneLayout::GetFrameLO(int width, int height) const {
	return window->GetFrame(width, height);
}
std::vector<std::vector<Pixel>> PaneLayout::GetFrameLO(int width, int height, IWindow* active) const {//!!!!!!!!!!!!!!!!!!!!!!!
	Color borderfg = Compare(active) ? BorderGetSelectFG() : BorderGetFG();
	Color borderbg = Compare(active) ? BorderGetSelectBG() : BorderGetBG();
	BorderType borderType = Compare(active) ? BorderGetSelectType() : BorderGetType();
	int thickness = BorderGetThickness();
	Border border = Border(borderType, borderfg, borderbg, thickness);//!!!!!!!!!!!!!!!!!!!!!!!!!
	if (width < thickness*2 || height < thickness*2) {
		return {};
	}
	return border.PasteInFrame(window->GetFrame(width - thickness*2, height - thickness*2));
}

std::vector<ILayout*> PaneLayout::GetPaneList() const  {
	return { (ILayout*)this };
}
std::vector<ILayout*> PaneLayout::GetPath(IWindow* window) const  {
	if (Compare(window)) {
		return { (ILayout*)this };
	}
	if (this == window) {
		return { (ILayout*)this };
	}
	return {};
}

void PaneLayout::UpdateBinds() {
	binds.ClearAll();
	binds.Add(window->GetBinds());
}
void PaneLayout::UpdateBinds(IWindow* active){
	binds.ClearAll();
	if (Compare(active))
		binds.Add(window->GetBinds());
}