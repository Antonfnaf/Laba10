#include "Core/enums.h"
#include "Core/Pixel.h"

//#include "UIEngine/composition/OldWindowCompositor.h"
#include "UIEngine/composition/ScreenLayout.h"
#include "UIEngine/rendering/Renderer.h"
#include "UIEngine/input/InputManager.h"



class WindowManager {
private:

public:
	static std::unique_ptr<IWindow> nullwin;
	static ScreenLayout screen;
	static std::vector<std::unique_ptr<IWindow>> windows;
	static std::map<std::string, IWindow*> bookmarks;
	static Bind binds;
	static Bind tempBinds;


	//Просто инициализация. здесь нет статических конструкторов насколько я понял
	static void Init() {
		screen = ScreenLayout(std::make_unique<PaneLayout>());
		std::vector<std::unique_ptr<IWindow>> windows = std::vector<std::unique_ptr<IWindow>>();
		std::map<std::string, IWindow*> bookmarks = std::map<std::string, IWindow*>();
		binds = Bind();
		tempBinds = Bind();
	}


	static std::map<KeyCode, std::function<void()>> GetBinds() {
		Bind temp;
		temp.Add(screen.GetBinds());
		temp.Add(tempBinds.GetCopy());
		temp.Add(binds.GetCopy());
		tempBinds.ClearAll();
		return temp.GetCopy();
	}

	//Выполнение действия по клавише.
	static void UpdateActions() {
		InputManager::ClearBinds();
		InputManager::Bind(GetBinds());
		InputManager::Do();
	}

	//отрисовка кадра
	static void UpdateCadre() {
		Renderer::UpdateCadre(screen.GetScreen(ConsoleTools::GetConsoleWidth(), ConsoleTools::GetConsoleHeight()));
	}

	static IWindow* GetWindow(int index) { return windows[index].get(); }
	static IWindow* GetWindow(std::string bookmark) { return bookmarks[bookmark]; }
	static void AddWindow(std::string bookmark, std::unique_ptr<IWindow> window) { 

		bookmarks[bookmark] = window.get();
		windows.push_back(std::move(window));
		return;
	}
	static void OpenWindow(std::string bookmark) {
		
		screen.SetActive(GetWindow(bookmark));
	}
	static void CloseWindow() {
		screen.DeleteActive();
	}
	static void RemoveWindow(const std::string& bookmark) {
		auto it = bookmarks.find(bookmark);
		if (it == bookmarks.end()) return;

		IWindow* raw = it->second;
		// найти окно в векторе
		for (auto& w : windows) {
			if (w.get() == raw) {
				windows.erase(std::remove(windows.begin(), windows.end(), w), windows.end());
				break;
			}
		}
		bookmarks.erase(it);
	}
	
};