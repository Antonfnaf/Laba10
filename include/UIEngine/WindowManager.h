#include "Core/enums.h"
#include "Core/Pixel.h"

//#include "UIEngine/composition/OldWindowCompositor.h"
#include "UIEngine/composition/ScreenLayout.h"
#include "UIEngine/rendering/Renderer.h"
#include "UIEngine/input/InputManager.h"



struct WindowManager {

	static ScreenLayout screen;
	static Bind binds;
	static Bind tempBinds;

	static void Init(std::unique_ptr<IWindow> rootWindow) {
		screen = ScreenLayout(std::make_unique<PaneLayout>(std::move(rootWindow)));
		binds = Bind();
		tempBinds = Bind();
	}

	static std::map<KeyCode, std::function<void()>> GetBinds() {
		Bind temp;
		temp.Add(screen.GetBinds());
		temp.Add(tempBinds.GetCopy());
		temp.Add(binds.GetCopy());
		tempBinds.Clear();
		return temp.GetCopy();
	}
	static void UpdateCadre() {
		Renderer::UpdateCadre(screen.GetScreen(ConsoleTools::GetConsoleWidth(), ConsoleTools::GetConsoleHeight()));
	}
	static void UpdateActions() {
		InputManager::ClearBinds();
		InputManager::Bind(GetBinds());
		InputManager::Do();
	}
	static void OpenWindow(std::unique_ptr<IWindow> window) {
		screen.SetActive(std::move(window));
	}

	//static void UpdateCadre() {
	//	Renderer::UpdateCadre(OldWindowCompositor::GetCadre());
	//}

	//static void SetWindow(Slot slot, IWindow* window) {
	//	OldWindowCompositor::SetWindow(slot, window);
	//}
	//static void SetFrame(Slot slot, BorderType type, int thickness, Color fg, Color bg) {
	//	OldWindowCompositor::SetFrame(slot, type, thickness, fg, bg);
	//}
	//static void SetFrameType(Slot slot, BorderType type) {
	//	OldWindowCompositor::SetFrameType(slot, type);
	//}
	//static void SetFrameThickness(Slot slot, int thickness) {
	//	OldWindowCompositor::SetFrameThickness( slot, thickness);
	//}
	//static void SetFrameBg(Slot slot, Color color) {
	//	OldWindowCompositor::SetFrameBg(slot, color);
	//}
	//static void SetFrameFg(Slot slot, Color color) {
	//	OldWindowCompositor::SetFrameFg(slot, color);
	//}

};