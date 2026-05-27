#pragma once
#include <vector>
#include <utility>

#include "Core/Pixel.h"
#include "UIEngine/composition/ILayout.h"
#include "UIEngine/composition/PaneLayout.h"
#include "UIEngine/composition/SplitLayout.h"


class ScreenLayout final : public ILayout {
	std::unique_ptr<ILayout> root;
	
	IWindow* active;
	int screenWidth;
	int screenHeight;


	Bind userBinds;
	bool defaultBindsIsOn = true;

	//Дает размеры искомого лейаута внутри корня
	std::pair<int, int> FindLayoutSize(ILayout* layout) const ;
	SplitLayout* FindRootSplit(const std::vector<ILayout*>& path, Direction dir) const;
	
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override {
		if(root.get())
			return root->GetFrame(width, height, active);
		return {};
	}
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		return GetFrameLO(width, height, active);
	}

	//void SwapLayouts(ILayout* a, ILayout* b);
	//обновление списка клавиш
	void UpdateBinds();
	void UpdateBinds(IWindow* window) { UpdateBinds(); }

public:
	ScreenLayout(std::unique_ptr<ILayout> layout) : root(std::move(layout)), active((!root->GetPaneList().empty()) ? root->GetPaneList()[0] : nullptr) {}
	
	//Дает размеры искомого лейаут43а внутри выбранного лейаута
	static std::pair<int, int> FindLayoutSize(ILayout* layoutToFind, ILayout* rootLayout, int rootWidth,int rootHeight);

	//позволяет получить клавиши скрина и активного окна
	std::map<KeyCode, std::function<void()>> GetBinds() {
		binds.Clear();

		UpdateBinds();
		return binds.GetCopy();
	}
	//Позволяет получить кадр скрина
	std::vector<std::vector<Pixel>> GetScreen(int width, int height) {
		screenWidth = width;
		screenHeight = height;
		return GetFrame(width, height, active);
	};


	void SetFocusPane(ILayout* pane);
	//изменение фокуса на определенное количество панелей
	void ChangeFocus(int num);
	//Изменение фокуса на следующую панель
	void FocusNext();
	//Изменение фокуса на предыдущую панель
	void FocusPrev();

	//Удаление панели 
	void DeletePane(IWindow* pane);
	//Удаление активной панели
	void DeleteActive();
	//Меняет содержимое активного окна
	void SetActive(IWindow* window);
	//Позволяет сделать сплит с пустым или выбранным окном
	void SplitActive(IWindow* window, bool vertical = false);

	//Позволяет изменить долю активного окна по выбранному направлению
	void ResizeActiveRatio(float newRatio, Direction direction);
	//Позволяет изменить долю активного окна на количество пикселей по выбранному направлению
	void ChangeActiveRatio(float ratioplus, Direction direction);
	//Позволяет изменить активное окно на один пиксель по выбранному направлению в большую сторону или меньшую
	bool ResizeActiveToOnePixel(Direction direction, bool pos);

	void DefaultBindsDisable() { defaultBindsIsOn = false; }
	void DefaultBindsEnable() { defaultBindsIsOn = true; }

	void AddUserBinds(KeyCode key, std::function<void()> action) { userBinds.Add(key, action); }
	void AddUserBinds(std::map<KeyCode, std::function<void()>> actions) { userBinds.Add(actions); }

	//Дает панель окна
	PaneLayout* GetPane(IWindow* window) {
		if (!root.get()) return nullptr;
		std::vector<ILayout*> path = root->GetPath(window);
		if (path.empty()) return nullptr;
		PaneLayout* pane = dynamic_cast<PaneLayout*>(path[0]);
		if (!pane) return nullptr;
		return pane;
	}
	//Дает список окон
	std::vector<ILayout*> GetPaneList() { return root->GetPaneList(); }
	//Дает список лейаутов на пути к окну, нулевой элемент - окно
	std::vector<ILayout*> GetPath(IWindow* window) override { return root->GetPath(window); }

};
