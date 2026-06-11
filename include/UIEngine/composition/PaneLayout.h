#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <optional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/ILayout.h"
#include "UIEngine/input/Bind.h"

#include "UIEngine/composition/Border.h"//!!!!!!!!!!!!!!!!!!!!!!!!!!



class PaneLayout : public ILayout
{
private:
	//Указатель на хранимое окно
	IWindow* window;

	static Color defaultBorderBG;
	static Color defaultSelectBorderBG;
	static Color defaultBorderFG;
	static Color defaultSelectBorderFG;
	static BorderType defaultBorderType;
	static BorderType defaultSelectBorderType;
	static int defaultBorderThickness;

	std::optional<Color> customBorderBG;
	std::optional<Color> customSelectBorderBG;
	std::optional<Color> customBorderFG;
	std::optional<Color> customSelectBorderFG;
	std::optional<BorderType> customBorderType;
	std::optional<BorderType> customSelectBorderType;
	std::optional<int> customBorderThickness;


	//Возвращает картинку обьекта под заданные размеры.
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override;
	//Возвращает картинку обьекта под заданные размеры с передачей активного окна(от ILayout).
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const override;

	//Обновление действий при каждом кадре.
	void UpdateBinds() override;
	//Обновление действий при каждом кадре, с зависимостью от активного обьекта.
	void UpdateBinds(IWindow* active) override;

public:
	PaneLayout() : window(nullptr) {}
	PaneLayout(IWindow* nwindow) : window(nwindow) {}

	static void BorderSetDefaultBG(Color value) { defaultBorderBG = value;}
	static void BorderSetDefaultSelectBG(Color value) { defaultSelectBorderBG = value;}
	static void BorderSetDefaultFG(Color value) { defaultBorderFG = value;}
	static void BorderSetDefaultSelectFG(Color value) { defaultSelectBorderFG = value;}
	static void BorderSetDefaultType(BorderType value) { defaultBorderType = value; };
	static void BorderSetDefaultSelectType(BorderType value) { defaultSelectBorderType = value; };
	static void BorderSetDefaultThickness(int value) {defaultBorderThickness = value;}

	void BorderSetCustomBG(Color value) { customBorderBG = value;}
	void BorderSetCustomSelectBG(Color value) { customSelectBorderBG = value;}
	void BorderSetCustomFG(Color value) { customBorderFG = value;}
	void BorderSetCustomSelectFG(Color value) { customSelectBorderFG = value;}
	void BorderSetCustomType(BorderType value) { customBorderType = value; };
	void BorderSetCustomSelectType(BorderType value) { customSelectBorderType = value; };
	void BorderSetCustomThickness(int value) { customBorderThickness = value; }

	void BorderResetToGlobalBG(Color value) { customBorderBG.reset();}
	void BorderResetToGlobalSelectBG(Color value) { customSelectBorderBG.reset();}
	void BorderResetToGlobalFG(Color value) { customBorderFG.reset();}
	void BorderResetToGlobalSelectFG(Color value) { customSelectBorderFG.reset();}
	void BorderResetToGlobalType(BorderType value) { customBorderType.reset(); };
	void BorderResetToGlobalSelectType(BorderType value) { customSelectBorderType.reset(); };
	void BorderResetToGlobalThickness(int value) { customBorderThickness.reset();}

	Color BorderGetBG() const { return customBorderBG.value_or(defaultBorderBG);}
	Color BorderGetSelectBG() const { return customSelectBorderBG.value_or(defaultSelectBorderBG);}
	Color BorderGetFG() const { return customBorderFG.value_or(defaultBorderFG);}
	Color BorderGetSelectFG() const { return customSelectBorderFG.value_or(defaultSelectBorderFG);}
	BorderType BorderGetType() const { return customBorderType.value_or(defaultBorderType); };
	BorderType BorderGetSelectType() const { return customSelectBorderType.value_or(defaultSelectBorderType); };
	int	BorderGetThickness() const { return customBorderThickness.value_or(defaultBorderThickness);}

	//Говорит активно ли хранимое окно
	bool const Compare(IWindow* active) const {
		return this == active;
	}

	//Присваивает новое окно
	void SetWindow(IWindow* newWindow) {
		window = newWindow;
	}
	//Возвращает список вложенных панелей
	std::vector<ILayout*> GetPaneList() const override;
	//Возвращает список лейаутов до искомого окна
	std::vector<ILayout*> GetPath(IWindow* window) const override;

	//-- from ILayout --
	// Bind binds
	// virtual GetFrameLO(int width, int height)
	// GetFrame(int width, int height)
	// GetFrame(int width, int height, IWindow* active)
	// GetBinds();
	// GetBinds(IWindow* active);
};


