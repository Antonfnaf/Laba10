#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Core/enums.h"
#include "Core/Pixel.h"
#include "UIEngine/composition/IWindow.h"
#include "UIEngine/input/Bind.h"

class ILayout : public IWindow {
protected:
	virtual std::vector<std::vector<Pixel>> GetFrameLO(int width, int height, IWindow* active) const = 0;
	
	virtual void UpdateBinds() = 0;
	virtual void UpdateBinds(IWindow* active) = 0;
	static const int MINSIZE = 2;


public: 
	std::map<KeyCode, std::function<void()>> GetBinds() {
		binds.Clear();
		UpdateBinds();
		return binds.GetCopy();
	}
	std::map<KeyCode, std::function<void()>> GetBinds(IWindow* active) {
		binds.Clear();
		UpdateBinds(active);
		return binds.GetCopy();
	}
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
	virtual std::vector<IWindow*> GetWindows() const = 0;
	virtual std::vector<ILayout*> GetPath(IWindow* window) = 0;
};

