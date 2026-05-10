#pragma once
#include <iostream>
#include <time.h>

#include "UIEngine/composition/IWindow.h"
#include "UIEngine/WindowManager.h"


#include "UIEngine/input/InputManager.h"

class TestWindow1 : public IWindow {
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		std::vector<std::vector<Pixel>> pic;
		for (int i = 0; i < height; i++)
		{
			pic.push_back(std::vector<Pixel>());
			for (int j = 0; j < width; j++)
			{
				int a = j % 4;
				switch (a) {
				case 0:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::Yellow));
					break;
				case 1:
					pic[i].push_back(Pixel("\u2665", Color::Red, Color::Yellow));
					break;
				case 2:
					pic[i].push_back(Pixel("\u2666", Color::Red, Color::Yellow));
					break;
				case 3:
					pic[i].push_back(Pixel("\u2663", Color::Black, Color::Yellow));
					break;
				default:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::Yellow));
					break;
				}
			}
		}
		return pic;
	}
};

class TestWindow2 : public IWindow {
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		std::vector<std::vector<Pixel>> pic;
		srand(time(NULL));
		for (int i = 0; i < height; i++)
		{
			pic.push_back(std::vector<Pixel>());
			for (int j = 0; j < width && j < rand() % width; j++)
			{
				int a = j % 4;
				switch (a) {
				case 0:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::White));
					break;
				case 1:
					pic[i].push_back(Pixel("\u2665", Color::Red, Color::White));
					break;
				case 2:
					pic[i].push_back(Pixel("\u2666", Color::Red, Color::White));
					break;
				case 3:
					pic[i].push_back(Pixel("\u2663", Color::Black, Color::White));
					break;
				default:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::White));
					break;
				}
			}
		}
		return pic;
	}
};

class TestMainWindow : public IWindow {
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		std::vector<std::vector<Pixel>> pic = std::vector<std::vector<Pixel>>();
		for (int i = 0; i < height; i++)
		{
			pic.push_back(std::vector<Pixel>());
			for (int j = 0; j < width; j++)
			{
				int a = j % 4;
				switch (a) {
				case 0:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::Green));
					break;
				case 1:
					pic[i].push_back(Pixel("\u2665", Color::Red, Color::Green));
					break;
				case 2:
					pic[i].push_back(Pixel("\u2666", Color::Red, Color::Green));
					break;
				case 3:
					pic[i].push_back(Pixel("\u2663", Color::Black, Color::Green));
					break;
				default:
					pic[i].push_back(Pixel("\u2660", Color::Black, Color::Green));
					break;
				}
			}
		}
		return pic;
	}
};

class Test1 {
private:

public:

	static void StartTest(int time) {
		//WindowManager::Init(std::make_unique<TestWindow1>());
		bool WindowChoise = false;
		WindowManager::binds.Add(KeyCode::Escape,[&]() {time = 0; });
		WindowManager::binds.Add(KeyCode::CtrlW,[&]() {WindowChoise = true; });

		for (int i = 0; i < time; i++)
		{
			if (true) {
				WindowManager::tempBinds.Add({
					{KeyCode::Digit1, [&]() {WindowManager::OpenWindow(std::make_unique<TestWindow1>()); WindowChoise = false; }},
					{KeyCode::Digit2, [&]() {WindowManager::OpenWindow(std::make_unique<TestWindow2>()); WindowChoise = false; }},
					{KeyCode::Digit3, [&]() {WindowManager::OpenWindow(std::make_unique<TestMainWindow>()); WindowChoise = false; }},
					});
			}
			WindowManager::UpdateActions();
			WindowManager::UpdateCadre();
		}
	}
};
				/*{KeyCode::Escape,[&]() {i = time; }},*/