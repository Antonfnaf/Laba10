#pragma once
#include <cstring>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "Core/enums.h"

struct InputEvent {
    int code;       // Код клавиши if -2 see raw_sequence
    int modifiers;  // Маска модификаторов
    bool is_release; // true if key release event, false if key press
    std::array<uint8_t, 32> raw_sequence; // Сырая последовательность, если нужно
    int raw_length; // Длина сырой последовательности
};
struct KeyEvent {
    Key key;       // Клавиша
    int modifiers;  // Маска модификаторов
    bool is_release; // true if key release event, false if key press
};

class InputHandler {
private:
    struct termios old_tio;
    int support_level = 0;


    // int parse_to_int(std::string str) {
    //     int res = 0;
    //     int i = 0;
    //     while (i < str.length() && (int)str[i] >= 0x30 && (int)str[i] < 0x3A ) {
    //         res*=10;
    //         res+=(int)str[i++]-48;
    //     }
    //     return i < str.length() ? -1 : res;
    // }
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

        CheckSupport();
    }

    ~InputHandler() {
        
        // Restore the terminal settings
        if (support_level > 0) {
            std::cout << "\x1b[<4m";
        }
        std::cout << "\x1b[>0u";
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

    void CheckSupport() {
        std::cout << "\x1b[>13u"; // Запрос поддержки Kitty Protocol 2
        std::cout.flush();
        std::cout << "\x1b[?u"; // Запрос версии
        std::cout.flush();

        char buf[16] = {0};
        struct pollfd pfd = { .fd = STDIN_FILENO, .events = POLLIN };
        
        // Ждем ответа 100мс
        if (poll(&pfd, 1, 100) > 0) {
            read(STDIN_FILENO, buf, sizeof(buf));
            // Ищем подстроку "?2u" или "?1u"
            int i = 0,num = 0;
            bool found = false;

            while (i < sizeof(buf) - 1 && buf[i] != '\0'){
                if (found){
                    if (buf[i] == 'u') break;
                    if (buf[i] >= '0' && buf[i] <= '9') {
                        num = num * 10 + (buf[i] - '0');
                    }
                    i++;
                    continue;
                }
                i++;
                if (buf[i] == '?') found = true;
            }
            support_level = num; //0-31
        }
    }
        
    int GetSupportLevel() const {
        return support_level;
    }

    InputEvent GetInputEvent() {
        InputEvent event = {0, 0, false, {0}, 0};

        if (!kbhit()) {
            return {-1, 0, false, {0}, 0}; // No key pressed
        }
        int code = getch();
        if (code == -1) {
            return {-1, 0, false, {0}, 0}; // No key pressed
        }
        uint8_t first = code;
        
        // 1. Обычный символ (не Escape)
        if (first != 0x1B) {
            return {first, 0, false, {first}, 1}; // Считаем обычным нажатием
        }
        std::array<uint8_t, 32> seq = {0x1B};
        int seq_len = 1;
        // 2. Начало последовательности CSI
        uint8_t second = getch();
        if (second == '[') {
            seq[1] = '[';
            seq_len++;
            // Читаем до финального символа (обычно 'u' для Kitty или '~', 'A' для старых)
            char ch;
            while (seq_len < 31 && read(STDIN_FILENO, &ch, 1) == 1) {
                seq[seq_len++] = ch;
                if (ch == 'u' || ch == '~' || (ch >= 'A' && ch <= 'Z')) break;
            }
            // Проверка на Kitty Protocol (заканчивается на 'u')
            bool kitty_format = (
                seq_len > 3 &&
                seq[0] == 0x1B &&
                seq[1] == '[' &&
                (seq[seq_len - 1] == 'u' ||
                seq[seq_len - 1] == '~' ||
                (seq[2] =='1' && seq[seq_len - 1] >= 'P' && seq[seq_len - 1] <= 'S') ||
                (seq[2] =='1' && seq[seq_len - 1] >= 'A' && seq[seq_len - 1] <= 'D')));

            if ( kitty_format) 
            {
                // Парсим Kitty формат: "code;mods:type"
                // Убираем возможные лишние символы если нужно
                int code = 0, mods = 0;
                bool release = false;
                
                // Пример строки внутри seq: "97;5:3"
                int i = 2; // Начинаем после "\x1b["
                while (i< seq_len && (seq[i] >= '0' && seq[i] <= '9')) {
                    code = code * 10 + (seq[i] - '0');
                    i++; // Читаем код клавиши
                }
                if (i < seq_len && seq[i] == ':') {
                    int number = 0;
                    i++;
                    while (i< seq_len && (seq[i] >= '0' && seq[i] <= '9')) {
                        number = number * 10 + (seq[i] - '0');
                        i++; // Читаем модификаторы
                    }
                    if (number == 3){
                        release = true; // 3 означает отпускание клавиши
                    }; 
                }
                if (i < seq_len && seq[i] == ';') {
                    i++;
                    while (i< seq_len && (seq[i] >= '0' && seq[i] <= '9')) {
                        mods = mods * 10 + (seq[i] - '0');
                        i++; // Читаем модификаторы
                    }
                    if (mods > 16) mods = 16; // Ограничение на модификаторы
                }
                if (i < seq_len && seq[i] == ':') {
                    i++;
                    if (i < seq_len && (seq[i] >= '0' && seq[i] <= '9')) {
                        release = (seq[i] - '0') == 3; // 3 означает отпускание клавиши
                    }
                }
                
                return {code, mods, release, seq, seq_len};
            } 
            // Неизвестная последовательность
            return {-2, 0, false, seq, seq_len}; // Нужно парсить отдельно
            
            
            // Здесь случаи старых кодов (\x1b[A и т.д.)
            return {-2, 0, false, seq, seq_len}; // Старый код, нужно парсить отдельно
        } 
        else if (second == 'O') {
            seq[1] = 'O';
            seq_len++;
            char ch;
            // Читаем третий байт (P, Q, R, S, A, B...)
            if (read(STDIN_FILENO, &ch, 1) == 1) {
                seq[seq_len++] = ch;
            }
            return {-2, 0, false, seq, seq_len}; // нужно парсить отдельно
        }
        
        return {0x1B, 0, false, {0x1B}, 1}; // Просто Esc
    }
    
    KeyEvent GetKeyEvent(InputEvent event) {
        if (event.code == -1) {
            return {Key::None, event.modifiers-1, event.is_release}; // No key pressed
        }
        if (event.code == -2) {
            // Обработка старых кодов
            // здесь используется парсинг event.raw_sequence для определения Key и модификаторов.
            // Преобразуем сырые байты в строку для удобства сравнения
            std::string seq(reinterpret_cast<char*>(event.raw_sequence.data()), event.raw_length);
            
            // Убираем общий префикс \x1b[ если он есть, чтобы сравнивать "хвост"
            // Но в linux terminfo некоторые коды начинаются с \x1b[[ (две скобки)
            
            Key key = Key::None;
            int mods = event.modifiers - 1; // В legacy режиме модификаторы часто уже "вшиты" в код или отсутствуют
            
            // Навигация
            if (seq == "\x1b[A") key = Key::UpArrow;
            else if (seq == "\x1b[B") key = Key::DownArrow;
            else if (seq == "\x1b[C") key = Key::RightArrow;
            else if (seq == "\x1b[D") key = Key::LeftArrow;
            
            // Editing keypad (Home, End, Del, Ins)
            else if (seq == "\x1b[1~") key = Key::Home;
            else if (seq == "\x1b[4~") key = Key::End;
            else if (seq == "\x1b[3~") key = Key::DeleteKey;
            else if (seq == "\x1b[2~") key = Key::Insert;
            else if (seq == "\x1b[5~") key = Key::PgUp;
            else if (seq == "\x1b[6~") key = Key::PgDn;
            
            // Function keys F1-F5 (специфично для linux console - двойная скобка)
            else if (seq == "\x1b[[A") key = Key::F1;
            else if (seq == "\x1b[[B") key = Key::F2;
            else if (seq == "\x1b[[C") key = Key::F3;
            else if (seq == "\x1b[[D") key = Key::F4;
            else if (seq == "\x1b[[E") key = Key::F5;
            
            // Function keys F6-F12
            else if (seq == "\x1bOP") key = Key::F1;
            else if (seq == "\x1bOQ") key = Key::F2;
            else if (seq == "\x1bOR") key = Key::F3;
            else if (seq == "\x1bOS") key = Key::F4;
            else if (seq == "\x1b[P") key = Key::F1;
            else if (seq == "\x1b[Q") key = Key::F2;
            else if (seq == "\x1b[R") key = Key::F3;
            else if (seq == "\x1b[S") key = Key::F4;
            else if (seq == "\x1b[15~") key = Key::F5;
            else if (seq == "\x1b[17~") key = Key::F6;
            else if (seq == "\x1b[18~") key = Key::F7;
            else if (seq == "\x1b[19~") key = Key::F8;
            else if (seq == "\x1b[20~") key = Key::F9;
            else if (seq == "\x1b[21~") key = Key::F10;
            else if (seq == "\x1b[23~") key = Key::F11;
            else if (seq == "\x1b[24~") key = Key::F12;
            
            // Shift+Tab (иногда приходит как ^I, но в terminfo linux указано kcbt=\E^I)
            // Это сложный случай, пока оставим None или обработаем отдельно
            
            return {key, mods, event.is_release};
        }
        bool kitty_format = (
            event.raw_sequence[1] == '[' &&
            event.raw_sequence[0] == 0x1B &&
            (event.raw_sequence[event.raw_length - 1] == 'u' ||
                event.raw_sequence[event.raw_length - 1] == '~' ||
                (event.raw_sequence[2] =='1' &&
                     event.raw_sequence[event.raw_length - 1] >= 'A' &&
                      event.raw_sequence[event.raw_length - 1] <= 'S')
            )
            );

        if (kitty_format) {
            Key key = Key::None;
            if (event.code < 0x20) {
                switch (event.code) {
                    case 0x01: {
                    uint8_t last = event.raw_sequence[event.raw_length - 1];
                    if (last == 'A') {key = Key::UpArrow; break;}
                    if (last == 'B') {key = Key::DownArrow; break;}
                    if (last == 'C') {key = Key::RightArrow; break;}
                    if (last == 'D') {key = Key::LeftArrow; break;}
                    if (last == 'H') {key = Key::Home; break;}
                    if (last == 'F') {key = Key::End; break;}
                    if (last == 'P') {key = Key::F1; break;}
                    if (last == 'Q') {key = Key::F2; break;}
                    if (last == 'R') {key = Key::F3; break;}
                    if (last == 'S') {key = Key::F4; break;}
                    }
                    break;
                    case 0x09: key = Key::Tab; break;
                    case 0x0D: key = event.raw_sequence[event.raw_length - 1] == '~' ? Key::F3 : Key::Enter; break;
                    case 0x1B: key = Key::Escape; break;
                    case 0x0F: key = Key::F5; break;
                    case 0x11: key = Key::F6; break;
                    case 0x12: key = Key::F7; break;
                    case 0x13: key = Key::F8; break;
                    case 0x14: key = Key::F9; break;
                    case 0x15: key = Key::F10; break;
                    case 0x17: key = Key::F11; break;
                    case 0x18: key = Key::F12; break;
                    default: key = Key::None; break;
                
                }
                
            }
            else if (event.code >= 0x20 && event.code < 0x80) {
                key = static_cast<Key>(event.code);
            } else if (event.code >= 0xE000 && event.code <= 0xE01F) {
                key = static_cast<Key>(event.code);
            } else if (event.code >= 0xE048 && event.code <= 0xE054) {
                key = static_cast<Key>(event.code);
            } else {
                key = Key::None; // Неизвестный код
            }
            // Здесь можно сопоставить event.code с Key enum,
            // в крайнем случае None
            return {key, event.modifiers-1, event.is_release};
        }
            // 3. Обработка однобайтовых символов (TTY / Raw Mode)
    int raw_code = event.code;
    int mods = static_cast<int>(event.modifiers -1);
    Key key = Key::None;

    // --- ПРИОРИТЕТ 1: Специальные управляющие клавиши ---
    switch (raw_code) {
        case 0x09: return {Key::Tab, mods, false};       // Tab (Ctrl+I)
        case 0x0A: return {Key::Enter, mods, false};     // Enter (Ctrl+M)        
        case 0x0D: return {Key::Enter, mods, false};     // Enter (Ctrl+M)
        case 0x1B: return {Key::Escape, mods, false};    // Esc (Ctrl+[)
        case 0x7F: return {Key::Delete, mods, false};    // Backspace (DEL)
    }

    // --- ПРИОРИТЕТ 2: Управляющие сsимволы Ctrl+A ... Ctrl+Z ---
    // Диапазон 0x01 - 0x1A
    if (raw_code >= 0x01 && raw_code <= 0x1A) {
        mods = mods | (int)Modifier::Ctrl; // Принудительно ставим флаг Ctrl
        // Превращаем код в букву: 0x01 -> 'A', 0x06 -> 'F' и т.д.
        char letter = 'a' + (raw_code - 1);
        
        // Проверяем, есть ли такая буква в твоем enum (у тебя они начинаются с 0x41)
        // В твоем enum Key::A = 0x61, Key::B = 0x62...
        key = static_cast<Key>(letter); 
        
        return {key, mods - 1, false};
    }

    // --- ПРИОРИТЕТ 3: Обычные печатные символы (ASCII 32-126) ---
    if (raw_code >= 32 && raw_code <= 126) {
        key = static_cast<Key>(raw_code);
        return {key, mods - 1, false};
    }

    return {Key::None, mods - 1, false};
    }
};