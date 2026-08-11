#include "Core/enums.h"
#include "Core/Pixel.h"

//#include "UIEngine/composition/OldWindowCompositor.h"
#include "UIEngine/composition/ScreenLayout.h"
#include "UIEngine/rendering/Renderer.h"
#include "UIEngine/input/InputManager.h"



class WindowManager {
private:
	static std::unique_ptr<IWindow> nullwin;
	static ScreenLayout screen;
	static std::vector<std::unique_ptr<IWindow>> windows;
	static std::map<std::string, IWindow*> bookmarks;
	static Bind binds;
	static Bind tempBinds;

public:


	//Просто инициализация. здесь нет статических конструкторов насколько я понял
	static void Init() {
		screen.SetDefaultBinds({

				{KeyCode::AltLeftArrow, [&]() {Screen_SwapLayouts(Direction::Left); }},
				{KeyCode::AltDownArrow, [&]() {Screen_SwapLayouts(Direction::Down); }},
				{KeyCode::AltUpArrow, [&]() {Screen_SwapLayouts(Direction::Up); }},
				{KeyCode::AltRightArrow, [&]() {Screen_SwapLayouts(Direction::Right); }},
				{KeyCode::CtrlLeftArrow, [&]() {
					if(!Screen_ChangeFocus(Direction::Left)) 
					if(!Screen_ChangeFocus(Direction::Right)) 
					Screen_FocusNext(); 
				}},
				{KeyCode::CtrlDownArrow, [&]() {
					if(!Screen_ChangeFocus(Direction::Down)) 
					if(!Screen_ChangeFocus(Direction::Up)) 
					Screen_FocusNext(); 
				}},
				{KeyCode::CtrlUpArrow, [&]() {
					if(!Screen_ChangeFocus(Direction::Up)) 
					if(!Screen_ChangeFocus(Direction::Down)) 
					Screen_FocusPrev(); 
				}},
				{KeyCode::CtrlRightArrow, [&]() {
					if(!Screen_ChangeFocus(Direction::Right)) 
					if(!Screen_ChangeFocus(Direction::Left)) 
					Screen_FocusPrev(); 
				}},
				{KeyCode::CtrlA, [&]() {Screen_SplitActive(new NullWindow(),false); }},
				{KeyCode::CtrlX, [&]() {Screen_SplitActive(new NullWindow(),true); }},
				{KeyCode::CtrlD, [&]() {Screen_DeleteActive(); }},
				{KeyCode::ShiftRightArrow, [&]() {
					if (!Screen_ResizeActiveByPixels(Direction::Right,1)) {
						Screen_ResizeActiveByPixels(Direction::Left, -1);
					}
				}},
				{KeyCode::ShiftLeftArrow, [&]() {
					if (!Screen_ResizeActiveByPixels(Direction::Left,1)) {
						Screen_ResizeActiveByPixels(Direction::Right, -1);
					}
				}},
				{KeyCode::ShiftDownArrow, [&]() {
					if (!Screen_ResizeActiveByPixels(Direction::Down, 1)) {
						Screen_ResizeActiveByPixels(Direction::Up, -1);
					}
				}},
				{KeyCode::ShiftUpArrow, [&]() {
					if (!Screen_ResizeActiveByPixels(Direction::Up, 1)) {
						Screen_ResizeActiveByPixels(Direction::Down, -1);
					}
				}},
			});
	}

	

	//============== ScreenLayout methods ==============
	//Дает соседний активному лейаут в заданном направлении
	static ILayout* Screen_FindLayoutByDirection(Direction dir) { return screen.FindLayoutByDirection(dir); }
	//изменение фокуса на определенную панель
	static bool Screen_SetFocusPane(ILayout* pane) { return screen.SetFocusPane(pane); }
	//изменение фокуса на определенное количество панелей
	static bool Screen_ChangeFocus(int num) { return screen.ChangeFocus(num); }
	//изменение фокуса на определенному направлению
	static bool Screen_ChangeFocus(Direction dir) { return screen.ChangeFocus(dir); }
	//Изменение фокуса на следующую панель
	static void Screen_FocusNext() { Screen_ChangeFocus(1); }
	//Изменение фокуса на предыдущую панель
	static void Screen_FocusPrev() { Screen_ChangeFocus(-1); }

	//Удаление панели 
	static void Screen_DeletePane(IWindow* pane) { screen.DeletePane(pane); }
	//Удаление активной панели
	static void Screen_DeleteActive() { screen.DeleteActive(); }
	//Меняет содержимое активного окна
	static void Screen_SetActive(IWindow* window) { screen.SetActive(window); }
	//Позволяет сделать сплит с пустым или выбранным окном
	static void Screen_SplitActive(IWindow* window, bool vertical = false) { screen.SplitActive(window, vertical); }

	//Меняет два лейаута местами.
	static void Screen_SwapLayouts(ILayout* a, ILayout* b) { screen.SwapLayouts(a, b); }
	static void Screen_SwapLayouts(Direction dir) { screen.SwapLayouts(dir); }

	//Позволяет изменить долю активного окна по выбранному направлению
	static void Screen_SetSplitRatio(SplitLayout* split, float newRatio) { screen.SetSplitRatio(split, newRatio); }
	//Позволяет изменить долю активного окна на количество процентов по выбранному направлению
	static void Screen_ChangeSplitRatio(SplitLayout* split, float delta) { screen.ChangeSplitRatio(split, delta); }
	//Позволяет изменить долю активного окна на количество пикселей по выбранному направлению
	static void Screen_ChangeSplitByPixels(SplitLayout* split, int delta) { screen.ChangeSplitByPixels(split, delta); }
	//Позволяет изменить активное окно на один пиксель по выбранному направлению в большую сторону или меньшую
	static bool Screen_ResizeActiveByPixels(Direction direction, int delta) { return screen.ResizeActiveByPixels(direction, delta); }

	static void Screen_DefaultBindsDisable() { screen.DefaultBindsDisable(); }
	static void Screen_DefaultBindsEnable() { screen.DefaultBindsEnable(); }

	//Позволяет добавить кастомное действие к клавише для скрина
	static void Screen_AddUserBinds(KeyCode key, std::function<void()> action) { screen.AddUserBinds(key, action); }
	//позволяет добавить несколько кастомных действий к клавишам для скрина
	static void Screen_AddUserBinds(std::map<KeyCode, std::function<void()>> actions) { screen.AddUserBinds(actions); }

	//============== Надстройки над ScreenLayout ==============
	//Открытие окна по закладке.
	static void OpenWindow(std::string bookmark) {
		Screen_SetActive(GetWindow(bookmark));
	}
	//Закрытие активного окна.
	static void CloseWindow() {
		Screen_DeleteActive();
	}


	//============== Windows list methods ==============
	//Получение окна по индексу или закладке.
	static IWindow* GetWindow(int index) { return windows[index].get(); }
	static IWindow* GetWindow(std::string bookmark) { return bookmarks[bookmark]; }
	//Добавление окна в менеджер окон с закладкой.
	static void AddWindow(std::string bookmark, std::unique_ptr<IWindow> window) { 

		bookmarks[bookmark] = window.get();
		windows.push_back(std::move(window));
		return;
	}
	//Удаление окна по закладке из менеджера окон.
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

	//Выполнение действия по клавише.

	//============== Binds methods ==============
	//Получение всех клавиш для скрина и активного окна
	static std::map<KeyCode, std::function<void()>> GetBinds(){
		Bind temp;
		temp.Add(screen.GetBinds());
		temp.Add(tempBinds.GetCopy());
		temp.Add(binds.GetCopy());
		return temp.GetCopy();
	}
	static void SetDefaultBinds(std::map<KeyCode, std::function<void()>> actions) { binds.ClearAll(); binds.Add(actions); }
	static void AddBinds(KeyCode key, std::function<void()> action) { tempBinds.Add(key, action); }
	static void AddBinds(std::map<KeyCode, std::function<void()>> actions) { tempBinds.Add(actions); }


	//
	static void UpdateActions() {
		InputManager::ClearBinds();
		InputManager::Bind(GetBinds());
		tempBinds.ClearAll();
		InputManager::Do();
	}
	//отрисовка кадра
	static void UpdateCadre() {
		Renderer::UpdateCadre(screen.GetScreen(ConsoleTools::GetConsoleWidth(), ConsoleTools::GetConsoleHeight()));
	}
	
};