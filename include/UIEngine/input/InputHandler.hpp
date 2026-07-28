#pragma once
#include <cstring>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "Core/enums.h"
#include "UIEngine/input//Bind.h"

struct KeyEvent {
    int code;       // Код клавиши if -2 see raw_sequence
    int modifiers;  // Маска модификаторов
    int event_type; // 0=None, 1=Press, 2=Repeat, 3=Release
    bool is_kitty;  // Был ли это Kitty-код
    std::string raw_sequence; // Сырая последовательность, если нужно
};

class InputHandler {
private:
    struct termios old_tio;
    int support_level = 0;


    int parse_to_int(std::string str) {
        int res = 0;
        int i = 0;
        while (i < str.length() && (int)str[i] >= 0x30 && (int)str[i] < 0x3A ) {
            res*=10;
            res+=(int)str[i++]-48;
        }
        return i < str.length() ? 0 : res;
    }
public:
    InputHandler() {
        // Save the terminal settings
        tcgetattr(STDIN_FILENO, &old_tio);

        // Set the terminal to raw mode
        struct termios new_tio = old_tio;

        new_tio.c_lflag &= ~(ICANON | ECHO | ISIG); // Disable canonical mode and echo
        new_tio.c_cc[VMIN] = 1; // Minimum number of characters to read
        new_tio.c_cc[VTIME] = 0; // Timeout (in deciseconds) for read
        
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio); // Apply the new settings

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Set non-blocking mode

        check_support();
    }

    ~InputHandler() {
        
        // Restore the terminal settings
        if (support_level > 0) {
            std::cout << "\x1b[<4m";
        }
        std::cout << "\x1b[?0u";
        std::cout << std::flush; 

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }

    int kbhit() {
        struct timeval tv = { 0L, 0L };
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
    }

    int getch() {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            return static_cast<unsigned char>(c);
        }
        return -1; // Return -1 if no input is available
    }

    void check_support() {
        std::cout << "\x1b[?u"; // Запрос версии
        std::cout.flush();

        char buf[16] = {0};
        struct pollfd pfd = { .fd = STDIN_FILENO, .events = POLLIN };
        
        // Ждем ответа 100мс
        if (poll(&pfd, 1, 100) > 0) {
            read(STDIN_FILENO, buf, sizeof(buf));
            // Ищем подстроку "?2u" или "?1u"
            if (strstr(buf, "?2u")) {
                support_level = 2;
                std::cout << "\x1b[?2u"; // Включаем явно, если поддержано
                std::cout.flush();
            }
            else if (strstr(buf, "?1u")) {
                support_level = 1;
                std::cout << "\x1b[?1u"; // Включаем явно, если поддержано
                std::cout.flush();
            } else {
                support_level = 0;
            }
        }
    }

    int get_support_level() const {
        return support_level;
    }

    KeyEvent get_key_event() {
        KeyEvent event = {0, 0, 0, false, ""};

        if (!kbhit()) {
            return {-1, 0, 0, false, ""}; // No key pressed
        }

        int first = getch();
        if (first == -1) {
            return {-1, 0, 0, false, ""}; // No key pressed
        }

        // 1. Обычный символ (не Escape)
        if (first != 0x1B) {
            return {first, 0, 1, false, std::string(1, static_cast<char>(first))}; // Считаем обычным нажатием
        }

        // 2. Начало последовательности CSI
    if (getch() == '[') {
        std::string seq;
        char ch;
        // Читаем до финального символа (обычно 'u' для Kitty или '~', 'A' для старых)
        while (read(STDIN_FILENO, &ch, 1) == 1) {
            seq += ch;
            if (ch == 'u' || ch == '~' || (ch >= 'A' && ch <= 'Z')) break;
        }

        // Проверка на Kitty Protocol (заканчивается на 'u')
        if (!seq.empty() && seq.back() == 'u') {
            // Парсим Kitty формат: "code;mods:type"
            // Убираем возможные лишние символы если нужно
            int code = 0, mods = 1, type = 1;
            
            // Простой парсинг через sscanf или вручную
            // Пример строки внутри seq: "97;5:3"
            char format[64];
            strcpy(format, seq.c_str());
            
            // Ищем двоеточие для типа события
            std::string s(seq);
            size_t colon_pos = s.find(':');
            size_t semi_pos = s.find(';');

            if (semi_pos != std::string::npos) {
                code = parse_to_int(s.substr(0, semi_pos));
                if (colon_pos != std::string::npos) {
                    mods = parse_to_int(s.substr(semi_pos + 1, colon_pos - semi_pos - 1));
                    type = parse_to_int(s.substr(colon_pos + 1));
                } else {
                    mods = parse_to_int(s.substr(semi_pos + 1));
                }
            } else if (colon_pos != std::string::npos) {
                code = parse_to_int(s.substr(0, colon_pos));
                type = parse_to_int(s.substr(colon_pos + 1));
            } else {
                code = parse_to_int(s);
            }
                
            return {code, mods, type, true, "\x1b[" + seq};
        }
        
        // Здесь случаи старых кодов (\x1b[A и т.д.)
        return {-2, 0, 1, false, "\x1b[" + seq}; 
    }

    return {0x1B, 0, 1, false, "\x1b"}; // Просто Esc
    }
};