#pragma once
#include <vector>
#include <utility>
#include <memory>

#include "Core/Pixel.h"
#include "UIEngine/composition/ILayout.h"
#include "UIEngine/composition/PaneLayout.h"
#include "UIEngine/composition/SplitLayout.h"

struct Place
{
	Place(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), w(w), h(h) {}
	int x, y, w, h;
};

class ScreenLayout final : public ILayout {
	std::unique_ptr<ILayout> root;
	
	ILayout* active;
	int screenWidth;
	int screenHeight;

	static Bind defaultBinds;
	Bind userBinds;
	bool defaultBindsIsOn = true;

	//Дает размеры искомого лейаута внутри корня
	Place FindLayoutPlace(ILayout* layout) const ;
	SplitLayout* FindSplitByDirection(const std::vector<ILayout*>& path, Direction dir) const;
	
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override {
			return root ? root->GetFrame(width, height, active) : std::vector<std::vector<Pixel>>();
	}
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return GetFrameLO(width, height, active);
	}
	//обновление списка клавиш
	void UpdateBinds();
	void UpdateBinds(IWindow* window) { UpdateBinds(); }

public:
	ScreenLayout(std::unique_ptr<ILayout> layout) : root(std::move(layout)), active((!root->GetPaneList().empty()) ? root->GetPaneList()[0] : nullptr) {}
	
	//Дает размеры искомого лейаута внутри выбранного лейаута
	static Place FindLayoutPlace(ILayout* layoutToFind, ILayout* rootLayout, int rootWidth,int rootHeight) ;
	// Возвращает окно/лейаут в направлении dir от выбранного окна.
	// Алгоритм: находим сплит, разделяющий выбранное окно и целевую область,
	// затем спускаемся по дереву в нужную сторону, используя центральную точку выбранного окна
	// для выбора правильной ветки (чтобы переходить в "логичное" окно, например,
	// из нижней левой панели в нижнюю правую, а не в верхнюю правую).
	ILayout* FindLayoutByDirection(ILayout* layoutFrom, Direction dir) const;
	// Возвращает окно/лейаут в направлении dir от активного окна.
	ILayout* FindLayoutByDirection(Direction dir) const;
	//Дает панель окна
	PaneLayout* GetPane(IWindow* window) const  {
		if (!root.get()) return nullptr;
		std::vector<ILayout*> path = root->GetPath(window);
		if (path.empty()) return nullptr;
		PaneLayout* pane = dynamic_cast<PaneLayout*>(path[0]);
		if (!pane) return nullptr;
		return pane;
	}
	//Дает список окон
	std::vector<ILayout*> GetPaneList() const { return root ? root->GetPaneList() : std::vector<ILayout*>(); }
	//Дает список лейаутов на пути к окну, нулевой элемент - окно
	std::vector<ILayout*> GetPath(IWindow* window) const override { return root ? root->GetPath(window) : std::vector<ILayout*>(); }

	//позволяет получить клавиши скрина и активного окна
	std::map<KeyCode, std::function<void()>> GetBinds() {
		binds.ClearAll();

		UpdateBinds();
		return binds.GetCopy();
	}
	//Позволяет получить кадр скрина
	std::vector<std::vector<Pixel>> GetScreen(int width, int height) {
		screenWidth = width;
		screenHeight = height;
		return GetFrame(width, height, active);
	};

	//изменение фокуса на определенную панель
	void SetFocusPane(ILayout* pane);
	//изменение фокуса на определенное количество панелей
	void ChangeFocus(int num);
	//изменение фокуса на определенному направлению
	void ChangeFocus(Direction dir);
	//Изменение фокуса на следующую панель
	void FocusNext();
	//Изменение фокуса на предыдущую панель
	void FocusPrev();

	//Удаление панели 
	void DeletePane(IWindow* pane);
	//Удаление активной панели
	void DeleteActive();
	//Меняет содержимое выбранного окна
	void SetContent(ILayout* target, IWindow* window);
	//Меняет содержимое активного окна
	void SetActive(IWindow* window);
	//Позволяет сделать сплит с пустым или выбранным окном
	void Split(ILayout* targetLayout, IWindow* window, bool vertical = false);
	//Позволяет сделать сплит с пустым или выбранным окном
	void SplitActive(IWindow* window, bool vertical = false);
	//Меняет два лейаута местами.
	void SwapLayouts(ILayout* a, ILayout* b);
	void SwapLayouts(ILayout* layoutFrom, Direction dir);
	void SwapLayouts(Direction dir);

	//Позволяет изменить долю активного окна по выбранному направлению
	void SetSplitRatio(SplitLayout* split, float newRatio);
	//Позволяет изменить долю активного окна на количество процентов по выбранному направлению
	void ChangeSplitRatio(SplitLayout* split, float delta);
	//Позволяет изменить долю активного окна на количество пикселей по выбранному направлению
	void ChangeSplitByPixels(SplitLayout* split, int delta);
	//Позволяет изменить выбранное окно на один пиксель по выбранному направлению в большую сторону или меньшую
	bool ResizeActiveByPixels(ILayout* layoutfrom, Direction direction, int delta);
	//Позволяет изменить активное окно на один пиксель по выбранному направлению в большую сторону или меньшую
	bool ResizeActiveByPixels(Direction direction, int delta);

	//Определяет дефолтные действия для скрина.
	static void SetDefaultBinds(std::map<KeyCode, std::function<void()>> binds) { defaultBinds.ClearAll(); defaultBinds.Add(binds); }
	//Позволяет добавить кастомное действие к клавише для скрина
	void AddUserBinds(KeyCode key, std::function<void()> action) { userBinds.Add(key, action); }
	//позволяет добавить несколько кастомных действий к клавишам для скрина
	void AddUserBinds(std::map<KeyCode, std::function<void()>> actions) { userBinds.Add(actions); }
	
	void DefaultBindsDisable() { defaultBindsIsOn = false; }
	void DefaultBindsEnable() { defaultBindsIsOn = true; }



};
