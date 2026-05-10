#pragma once
#include <iostream>
#include <cstdlib> 
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#endif




// инициализация utf-8 до всего
struct ConsoleInitializer {
    ConsoleInitializer()
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
    }
};

//глобальный инициализатор
inline ConsoleInitializer consoleInit;

struct ConsoleTools 
{
    static int GetConsoleWidth() 
    {
#ifdef _WIN32

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			return 0;

        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
		static std::pair<int, int> getVisibleSizeUnix() 
        {
			struct winsize w;
			if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) 
            {
					return w.ws_col;
			}
        }
#endif
    }
    static int GetConsoleHeight() {
#ifdef _WIN32

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
            return 0;

        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
        static std::pair<int, int> getVisibleSizeUnix() {
            struct winsize w;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
                return w.ws_row;
            }
        }
#endif
    }
};

struct FrameTools
{
    template<typename T>
    static void const Concat(std::vector<T>& a, const std::vector<T>& b) {
        a.insert(a.end(), b.begin(), b.end());
    }
    static std::vector<std::vector<Pixel>> const ConcatFrame(std::vector<std::vector<Pixel>> frame1, std::vector<std::vector<Pixel>> frame2, bool toX) {
        if (!toX) {
            Concat(frame1, frame2);
        } else {
            for (int i = 0; i < frame1.size() || i < frame2.size(); i++) {
                Concat(frame1[i], frame2[i]);
            }
        }
        return frame1;
    }
};

struct TestTools {
    static void DrawPixels(std::vector<std::vector<Pixel>> pixels, bool pause = false) {
        system("cls");
        for (int i = 0; i < pixels.size(); i++) {
            for (int j = 0; j < pixels[i].size(); j++) {
                std::cout << pixels[i][j].Get();
            }
            std::cout << '\n';
        }
        std::cout << std::flush;
        if (pause)
        {
            std::cin;
        }
    }
};
