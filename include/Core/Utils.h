#pragma once
#include <iostream>
#include <cstdlib> 
#include <vector>
#include <string>
#include <optional>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#endif

#include "Core/enums.h"
#include "Core/Pixel.h"




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

    //static std::vector<std::vector<Pixel>> StringToPic(std::string text) {
    //    std::vector<std::vector<Pixel>> pic;
    //    int bg = 0;
    //    int fg = 7;
    //    for (int i = 0; i < text.length(); i++) {
    //        if (text[i] != '\\') {
    //            bg = 0;
    //            fg = 7;
    //        } else {
    //            i++;


    //        }
    //    }
    //}
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






















// ===== Статический парсер =====
class PixelParser
{
public:
    static std::vector<std::vector<Pixel>> Parse(const std::string& input, bool appendReset = false) {
        std::vector<std::vector<Pixel>> rows;
        std::vector<Pixel> currentRow;
        std::string pendingStyles; // Накопитель стилей/сбросов для следующего символа

        size_t idx = 0;
        Color curFg = Color(15), curBg = Color(0);

        auto isStyleCode = [](int code) {
            return code == 1 || code == 2 || code == 3 || code == 4 || code == 5 ||
                code == 7 || code == 9 || code == 21 || code == 51 || code == 53;
            };
        auto isStyleResetCode = [](int code) {
            return code == 22 || code == 23 || code == 24 || code == 27 || code == 29;
            };

        // Парсит SGR, обновляет цвета и накапливает стили/сбросы в pendingStyles
        auto parseSGR = [&](const std::string& params) {
            if (params.empty()) {
                curFg = Color(15); curBg = Color(0);
                pendingStyles = "\033[0m";
                return;
            }

            std::vector<int> codes;
            size_t start = 0;
            for (size_t i = 0; i <= params.size(); ++i) {
                if (i == params.size() || params[i] == ';') {
                    if (i > start) {
                        try { codes.push_back(std::stoi(params.substr(start, i - start))); } catch (...) { return; }
                    }
                    start = i + 1;
                }
            }

            for (size_t i = 0; i < codes.size(); ++i) {
                int code = codes[i];
                if (code == 0) {
                    curFg = Color(15); curBg = Color(0);
                    pendingStyles = "\033[0m"; // ✅ Сброс заменяет все предыдущие pending-стили
                } else if (isStyleCode(code) || isStyleResetCode(code)) {
                    pendingStyles += "\033[" + std::to_string(code) + "m";
                } else if (code == 38 && i + 2 < codes.size() && codes[i + 1] == 5) {
                    int c = codes[i + 2]; if (c >= 0 && c <= 255) curFg = Color(c); i += 2;
                } else if (code == 48 && i + 2 < codes.size() && codes[i + 1] == 5) {
                    int c = codes[i + 2]; if (c >= 0 && c <= 255) curBg = Color(c); i += 2;
                } else if (code >= 30 && code <= 37) curFg = Color(code - 30);
                else if (code >= 40 && code <= 47) curBg = Color(code - 40);
                else if (code == 39) curFg = Color(15);
                else if (code == 49) curBg = Color(0);
                // Прочие коды игнорируются
            }
        };

        auto parseCSI = [&]() {
            idx += 2; // \033[
            std::string params;
            while (idx < input.size()) {
                char c = input[idx++];
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    if (c == 'm') parseSGR(params);
                    return; // Не-SGR команды игнорируются
                }
                params += c;
            }
        };

        auto getNextChar = [&]() -> std::string {
            if (idx >= input.size()) return "";
            unsigned char c = static_cast<unsigned char>(input[idx]);
            int len = (c & 0x80) == 0x00 ? 1 : (c & 0xE0) == 0xC0 ? 2 : (c & 0xF0) == 0xE0 ? 3 : (c & 0xF8) == 0xF0 ? 4 : 1;
            if (idx + len > input.size()) len = input.size() - idx;
            std::string res = input.substr(idx, len);
            idx += len;
            return res;
        };

        // Завершает строку: прикрепляет оставшиеся стили к последнему символу
        auto finalizeRow = [&](bool forceReset) {
            if (currentRow.empty()) return;
            if (!pendingStyles.empty()) {
                currentRow.back().symbol += pendingStyles;
                pendingStyles.clear();
            }
            if (forceReset) {
                currentRow.back().symbol += "\033[0m";
            }
            rows.push_back(std::move(currentRow));
        };

        // ===== Главный цикл =====
        while (idx < input.size()) {
            if (input[idx] == '\n') {
                finalizeRow(false); // \n переносит накопленные стили в конец прошлого символа
                ++idx; continue;
            }
            if (input[idx] == '\r') { ++idx; continue; }

            if (input[idx] == '\033' && idx + 1 < input.size() && input[idx + 1] == '[') {
                parseCSI();
                continue;
            }

            std::string ch = getNextChar();
            if (ch.empty()) break;

            currentRow.emplace_back(pendingStyles + ch, curFg, curBg);
            pendingStyles.clear(); // Стили "сгорают", привязавшись к символу
        }

        finalizeRow(appendReset);
        return rows;
    }
};