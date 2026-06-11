#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/IWindow.h"
#include "UIEngine/input/Bind.h"


//Интерфейс для окон хранящих другие окна и передающих информацию рекурсивно   
class ILayout : public IWindow {
protected:

	//Возвращает картинку обьекта под заданные размеры с передачей активного окна
	virtual std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const = 0;
	
	//Обновление действий при каждом кадре.
	virtual void UpdateBinds() = 0;
	//Обновление действий при каждом кадре, с зависимостью от активного обьекта.
	virtual void UpdateBinds(IWindow* active) = 0;
	static const int MINSIZE = 2;


public: 
	//Возвращает список вложенных панелей
	virtual std::vector<ILayout*> GetPaneList() const = 0;
	//Возвращает список лейаутов до искомого окна
	virtual std::vector<ILayout*> GetPath(IWindow* window) const = 0;
	//Безопасное получние действий
	std::map<KeyCode, std::function<void()>> GetBinds() {
		binds.ClearAll();
		UpdateBinds();
		return binds.GetCopy();
	}
	//Безопасное получние действий с зависимостью от активного обьекта
	std::map<KeyCode, std::function<void()>> GetBinds(IWindow* active) {
		binds.ClearAll();
		UpdateBinds(active);
		return binds.GetCopy();
	}
	//Немного измененная функция получения прямоугольной картинки
	std::vector<std::vector<Pixel>> GetFrame(int width, int height, IWindow* active) const {
		if (height < 0)
			height = 0;
		if (width < 0)
			width = 0; 

		std::vector<std::vector<Pixel>> frame = GetFrameLO(width, height, active);
		if (frame.size() < height)
			frame.insert(frame.end(), height - frame.size(), std::vector<Pixel>());
		else if (frame.size() > height)
			frame.erase(frame.begin() + height, frame.end());

		for (int i = 0; i < height; i++) {
			if (frame[i].size() < width)
				frame[i].insert(frame[i].end(), width - frame[i].size(), Pixel());
			else if (frame[i].size() > width)
				frame[i].erase(frame[i].begin() + width, frame[i].end());
		}
		return frame;
	};

	//-- from IWindow --
	// Bind binds
	// virtual GetFrameLO(int width, int height)
	// GetFrame(int width, int height)
	// GetBinds();
};

