#pragma once
#include <iostream>
#include <time.h>

#include "UIEngine/composition/IWindow.h"
#include "UIEngine/WindowManager.h"

#include "UIEngine/input/InputManager.h"

#include "UI/windows/WButtonList.h"
#include "UI/windows/WTerminal.h"








class TestWindow1 : public IWindow {
	std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const override {
		std::vector<std::vector<Pixel>> pic;
		//for (int i = 0; i < height; i++)
		//{
		//	pic.push_back(std::vector<Pixel>());
		//	for (int j = 0; j < width; j++)
		//	{
		//		int a = j % 4;
		//		switch (a) {
		//		case 0:
		//			pic[i].push_back(Pixel("\u2660", Color::Black, Color::Yellow));
		//			break;
		//		case 1:
		//			pic[i].push_back(Pixel("\u2665", Color::Red, Color::Yellow));
		//			break;
		//		case 2:
		//			pic[i].push_back(Pixel("\u2666", Color::Red, Color::Yellow));
		//			break;
		//		case 3:
		//			pic[i].push_back(Pixel("\u2663", Color::Black, Color::Yellow));
		//			break;
		//		default:
		//			pic[i].push_back(Pixel("\u2660", Color::Black, Color::Yellow));
		//			break;
		//		}
		//	}
		//}
		pic = Converter::ParseToPic("\x1b[48;05;123;38;05;215;01msdfg\u2345\x31\033[3md\x1b[0mgds\4\\\nergwe\033[3mojrigwoei\x1b[0mjrgergewrg");
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
		WindowManager::Init();
		bool WindowChoise = false;
		WindowManager::AddWindow("1", std::make_unique<TestWindow1>());
		WindowManager::AddWindow("2", std::make_unique<TestWindow2>());
		WindowManager::AddWindow("3", std::make_unique<TestMainWindow>());
		WindowManager::AddWindow(
			"4",
			std::make_unique<WButtonList>(
				"Window menu",
				std::vector<Button> {
					Button("Close window", [&]() {
						WindowManager::CloseWindow();
					}),
					Button("Split menu", [&]() {
						WindowManager::Screen_ChangeFocus(1);
						WindowManager::OpenWindow("Split menu");
						WindowManager::Screen_ChangeFocus(-1);
					}),
					Button("Open window 1", [&]() {
						WindowManager::Screen_ChangeFocus(1);
						WindowManager::OpenWindow("1");
						WindowManager::Screen_ChangeFocus(-1);
					}),
					Button("Open window 2", [&]() {
						WindowManager::Screen_ChangeFocus(1);
						WindowManager::OpenWindow("2");
						WindowManager::Screen_ChangeFocus(-1);
					}),
					Button("Open window 3", [&]() {
						WindowManager::Screen_ChangeFocus(1);
						WindowManager::OpenWindow("3");
						WindowManager::Screen_ChangeFocus(-1);
					}),
					Button(),
					Button(),
					Button(),
					Button(),

				}
			)
		);
		WindowManager::AddWindow(
			"Split menu",
			std::make_unique<WButtonList>(
				"Split menu",
				std::vector<Button> {
			Button("Close window", [&]() {
				WindowManager::CloseWindow();
				}),
				Button("Split menu", [&]() {
				WindowManager::Screen_ChangeFocus(1);
				WindowManager::OpenWindow("Split menu");
				WindowManager::Screen_ChangeFocus(-1);
					}),
				Button("Open window 1", [&]() {
				WindowManager::Screen_ChangeFocus(1);
				WindowManager::OpenWindow("1");
				WindowManager::Screen_ChangeFocus(-1);
					}),
				Button("Open window 2", [&]() {
				WindowManager::Screen_ChangeFocus(1);
				WindowManager::OpenWindow("2");
				WindowManager::Screen_ChangeFocus(-1);
					}),
				Button("Open window 3", [&]() {
				WindowManager::Screen_ChangeFocus(1);
				WindowManager::OpenWindow("3");
				WindowManager::Screen_ChangeFocus(-1);
					}),
		}
			)
		);


		WindowManager::SetDefaultBinds({ {KeyCode::Escape,[&]() {time = 0; }} });
		//std::unique_ptr buttonList1 =



		for (int i = 0; i < time; i++)
		{
			
			WindowManager::AddBinds({
				{KeyCode::Digit1, [&]() {WindowManager::OpenWindow("1"); WindowChoise = false; }},
				{KeyCode::Digit2, [&]() {WindowManager::OpenWindow("2"); WindowChoise = false; }},
				{KeyCode::Digit3, [&]() {WindowManager::OpenWindow("3"); WindowChoise = false; }},
				{KeyCode::Digit4, [&]() {WindowManager::OpenWindow("4"); WindowChoise = false; }},
				});


			WindowManager::Update();
		}
	}
};
				/*{KeyCode::Escape,[&]() {i = time; }},*/