#include "UIEngine/input/InputManager.h"
/////////////////
//#include <termios.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <iostream>
/////////////////
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <ncurses.h>//for linux, not implement
#endif



Bind InputManager::actions;

////////////////////////////////////
KeyCode parseChar(unsigned char c) {
    // Ctrl+буквы (0x01-0x1A) - возвращаем как Ctrl + буква
    // Но для этого нам нужно вернуть ДВЕ клавиши: LeftCtrl и букву
    // Так как функция возвращает одну клавишу, мы будем обрабатывать это 
    // в главной функции getKeyCodes()
    if (c >= 0x01 && c <= 0x1A) {
        // Это Ctrl+буква, но мы не можем вернуть две клавиши из одной функции
        // Поэтому в parseChar мы возвращаем специальное значение,
        // а в getKeyCodes обрабатываем это как Ctrl + буква
        return static_cast<KeyCode>(0xF000 + c);  // Временный маркер
    }

    // Управляющие символы
    if (c == 0x1B) return KeyCode::Escape;
    if (c == 0x0D) return KeyCode::Enter;
    if (c == 0x09) return KeyCode::Tab;
    if (c == 0x08) return KeyCode::Backspace;
    if (c == 0x7F) return KeyCode::Delete;

    // Печатные символы (ASCII)
    if (c >= 0x20 && c <= 0x7E) {
        return static_cast<KeyCode>(c);
    }

    return KeyCode::None;
}
//std::vector<KeyCode> getKeyCodes() {
//    std::vector<KeyCode> result;
//    std::unordered_set<KeyCode> unique_keys;
//
//    // Читаем все доступные данные из буфера
//    char buffer[1024];
//    int bytes = 0;
//
//    while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
//        int pos = 0;
//        while (pos < bytes) {
//            unsigned char c = static_cast<unsigned char>(buffer[pos]);
//
//            // Обработка ESC-последовательностей
//            if (c == 0x1B) {
//                std::string seq = parseEscapeSequence(buffer, bytes, pos);
//
//                // Проверяем, не Alt+буква ли это (ESC + буква)
//                if (seq.size() == 2 && seq[0] == 0x1B) {
//                    char letter = seq[1];
//                    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z')) {
//                        // Alt+буква - добавляем обе клавиши
//                        unique_keys.insert(KeyCode::LeftAlt);
//                        unique_keys.insert(static_cast<KeyCode>(letter));
//                    }
//                } else {
//                    KeyCode code = parseEscSequence(seq);
//                    if (code != KeyCode::None) {
//                        unique_keys.insert(code);
//                    }
//                }
//                pos += seq.length();
//            } else {
//                // Обычный символ
//                if (c >= 0x01 && c <= 0x1A) {
//                    // Ctrl+буква - добавляем Ctrl и букву
//                    unique_keys.insert(KeyCode::LeftCtrl);
//
//                    // Преобразуем 0x01->'A', 0x02->'B', ..., 0x1A->'Z'
//                    char letter = 'A' + (c - 0x01);
//                    unique_keys.insert(static_cast<KeyCode>(letter));
//                } else {
//                    KeyCode code = parseChar(c);
//                    if (code != KeyCode::None) {
//                        unique_keys.insert(code);
//                    }
//                }
//                pos++;
//            }
//        }
//    }
//    
//    // Добавляем модификаторы, которые удерживаются
//    auto current_time = std::chrono::steady_clock::now();
//    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
//        current_time - last_read
//    ).count();
//
//    if (elapsed < 50) {
//        for (auto key : previous_keys) {
//            if (isModifier(key)) {
//                unique_keys.insert(key);
//            }
//        }
//    }
//
//    last_read = current_time;
//    previous_keys = unique_keys;
//
//    result.reserve(unique_keys.size());
//    for (auto key : unique_keys) {
//        result.push_back(key);
//    }
//
//    return result;
//}
////////////////////////////////////


KeyCode InputManager::getKeyCode() {
    // ============================================================
    // 1. СНАЧАЛА ПРОВЕРЯЕМ МОДИФИКАТОРЫ (до вызова _getch!)
    // ============================================================
    bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

    // Проверяем Caps Lock (для букв)
    bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    // ============================================================
    // 2. ПРОВЕРЯЕМ НАЛИЧИЕ КЛАВИШИ В БУФЕРЕ
    // ============================================================
    if (!_kbhit()) return KeyCode::None;

    int first = _getch();

    // ============================================================
    // 3. ДВУХБАЙТОВЫЕ КЛАВИШИ (0x00 или 0xE0)
    // ============================================================
    if (first == 0x00 || first == 0xE0) {
        if (!_kbhit()) return KeyCode::None;
        int second = _getch();

        // Определяем базовую клавишу без модификаторов
        KeyCode baseKey = KeyCode::None;

        switch (second) {
            // Стрелки и навигация
        case 0x48: baseKey = KeyCode::UpArrow;    break;    // Up
        case 0x8d: baseKey = KeyCode::CtrlUpArrow; break;    // Ctrl+Up
        case 0x98: baseKey = KeyCode::AltUpArrow; break;    // Alt+Up
                   
        case 0x50: baseKey = KeyCode::DownArrow;  break;   // Down
        case 0x91: baseKey = KeyCode::CtrlDownArrow; break;  // Ctrl+Down
        case 0xA0: baseKey = KeyCode::AltDownArrow; break;  // Alt+Down
                   
        case 0x4B: baseKey = KeyCode::LeftArrow;  break;    // Left
        case 0x73: baseKey = KeyCode::CtrlLeftArrow; break;  // Ctrl+Left
        case 0x9B: baseKey = KeyCode::AltLeftArrow; break;   // Alt+Left
                   
        case 0x4D: baseKey = KeyCode::RightArrow; break;   // Right
        case 0x74: baseKey = KeyCode::CtrlRightArrow; break; // Ctrl+Right
        case 0x9D: baseKey = KeyCode::AltRightArrow; break; // Alt+Right

            // F1-F12
        case 0x3B: baseKey = KeyCode::F1; break;
        case 0x3C: baseKey = KeyCode::F2; break;
        case 0x3D: baseKey = KeyCode::F3; break;
        case 0x3E: baseKey = KeyCode::F4; break;
        case 0x3F: baseKey = KeyCode::F5; break;
        case 0x40: baseKey = KeyCode::F6; break;
        case 0x41: baseKey = KeyCode::F7; break;
        case 0x42: baseKey = KeyCode::F8; break;
        case 0x43: baseKey = KeyCode::F9; break;
        case 0x44: baseKey = KeyCode::F10; break;
        case 0x85: baseKey = KeyCode::F11; break;
        case 0x86: baseKey = KeyCode::F12; break;

        default: return KeyCode::None;
        }

        // Применяем модификаторы
        if (altPressed) {
            switch (baseKey) {
            case KeyCode::UpArrow: return KeyCode::AltUpArrow;
            case KeyCode::DownArrow: return KeyCode::AltDownArrow;
            case KeyCode::LeftArrow: return KeyCode::AltLeftArrow;
            case KeyCode::RightArrow: return KeyCode::AltRightArrow;
            case KeyCode::Home: return KeyCode::AltHome;
            case KeyCode::End: return KeyCode::AltEnd;
            case KeyCode::PgUp: return KeyCode::AltPgUp;
            case KeyCode::PgDn: return KeyCode::AltPgDn;
            case KeyCode::Insert: return KeyCode::AltTab;  // Alt+Insert = AltTab
            case KeyCode::DeleteKey: return KeyCode::AltTab;
            case KeyCode::F1: return KeyCode::AltF1;
            case KeyCode::F2: return KeyCode::AltF2;
            case KeyCode::F3: return KeyCode::AltF3;
            case KeyCode::F4: return KeyCode::AltF4;
            case KeyCode::F5: return KeyCode::AltF5;
            case KeyCode::F6: return KeyCode::AltF6;
            case KeyCode::F7: return KeyCode::AltF7;
            case KeyCode::F8: return KeyCode::AltF8;
            case KeyCode::F9: return KeyCode::AltF9;
            case KeyCode::F10: return KeyCode::AltF10;
            case KeyCode::F11: return KeyCode::AltF11;
            case KeyCode::F12: return KeyCode::AltF12;
            default: return baseKey;
            }
        }

        if (ctrlPressed) {
            switch (baseKey) {
            case KeyCode::UpArrow: return KeyCode::CtrlUpArrow;
            case KeyCode::DownArrow: return KeyCode::CtrlDownArrow;
            case KeyCode::LeftArrow: return KeyCode::CtrlLeftArrow;
            case KeyCode::RightArrow: return KeyCode::CtrlRightArrow;
            case KeyCode::Home: return KeyCode::CtrlHome;
            case KeyCode::End: return KeyCode::CtrlEnd;
            case KeyCode::PgUp: return KeyCode::CtrlPgUp;
            case KeyCode::PgDn: return KeyCode::CtrlPgDn;
            case KeyCode::Insert: return KeyCode::CtrlInsert;
            case KeyCode::DeleteKey: return KeyCode::CtrlDelete;
            case KeyCode::F1: return KeyCode::CtrlF1;
            case KeyCode::F2: return KeyCode::CtrlF2;
            case KeyCode::F3: return KeyCode::CtrlF3;
            case KeyCode::F4: return KeyCode::CtrlF4;
            case KeyCode::F5: return KeyCode::CtrlF5;
            case KeyCode::F6: return KeyCode::CtrlF6;
            case KeyCode::F7: return KeyCode::CtrlF7;
            case KeyCode::F8: return KeyCode::CtrlF8;
            case KeyCode::F9: return KeyCode::CtrlF9;
            case KeyCode::F10: return KeyCode::CtrlF10;
            case KeyCode::F11: return KeyCode::CtrlF11;
            case KeyCode::F12: return KeyCode::CtrlF12;
            default: return baseKey;
            }
        }

        if (shiftPressed) {
            switch (baseKey) {
            case KeyCode::UpArrow: return KeyCode::ShiftUpArrow;
            case KeyCode::DownArrow: return KeyCode::ShiftDownArrow;
            case KeyCode::LeftArrow: return KeyCode::ShiftLeftArrow;
            case KeyCode::RightArrow: return KeyCode::ShiftRightArrow;
            case KeyCode::Home: return KeyCode::ShiftHome;
            case KeyCode::End: return KeyCode::ShiftEnd;
            case KeyCode::PgUp: return KeyCode::ShiftPgUp;
            case KeyCode::PgDn: return KeyCode::ShiftPgDn;
            case KeyCode::Insert: return KeyCode::ShiftInsert;
            case KeyCode::DeleteKey: return KeyCode::ShiftDelete;
            case KeyCode::F1: return KeyCode::ShiftF1;
            case KeyCode::F2: return KeyCode::ShiftF2;
            case KeyCode::F3: return KeyCode::ShiftF3;
            case KeyCode::F4: return KeyCode::ShiftF4;
            case KeyCode::F5: return KeyCode::ShiftF5;
            case KeyCode::F6: return KeyCode::ShiftF6;
            case KeyCode::F7: return KeyCode::ShiftF7;
            case KeyCode::F8: return KeyCode::ShiftF8;
            case KeyCode::F9: return KeyCode::ShiftF9;
            case KeyCode::F10: return KeyCode::ShiftF10;
            case KeyCode::F11: return KeyCode::ShiftF11;
            case KeyCode::F12: return KeyCode::ShiftF12;
            default: return baseKey;
            }
        }

        return baseKey;
    }

    // ============================================================
    // 4. CTRL + БУКВЫ (0x01-0x1A)
    // ============================================================
    if (first >= 0x01 && first <= 0x1A) {
        // Ctrl+A = 0x01 ... Ctrl+Z = 0x1A
        if (ctrlPressed)
            switch (first) {
            case 0x08: return KeyCode::CtrlH;
            case 0x09: return KeyCode::CtrlI;
            case 0x0D: return KeyCode::CtrlM;
            default: break;
            }
        return static_cast<KeyCode>(static_cast<int>(KeyCode::CtrlA) + (first - 1));
    }
    if (ctrlPressed && first == 67) {
        return KeyCode::CtrlV;
    }

    // ============================================================
    // 5. ALT + БУКВЫ (проверяем через GetAsyncKeyState)
    // ============================================================
    if (altPressed) {
        // Проверяем буквы A-Z
        for (int i = 'A'; i <= 'Z'; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                return static_cast<KeyCode>(static_cast<int>(KeyCode::AltA) + (i - 'A'));
            }
        }
    }

    // ============================================================
    // 6. ОБЫЧНЫЕ СИМВОЛЫ (0x20-0x7E)
    // ============================================================
    // Буквы с учётом Shift и Caps Lock
    if (first >= 'A' && first <= 'Z') {
        bool upper = shiftPressed ^ capsLock;
        if (upper) {
            return static_cast<KeyCode>(first);  // Заглавная
        } else {
            return static_cast<KeyCode>(tolower(first));  // Строчная
        }
    }

    // Остальные символы (цифры, знаки препинания, пробел и т.д.)
    // Возвращаем как есть, т.к. значения совпадают с enum
    return static_cast<KeyCode>(first);
}

void InputManager::Bind(KeyCode key, std::function<void()> action) {
    actions.Add(key,action);
}
void InputManager::Bind(std::map<KeyCode, std::function<void()>> localActions) {
    actions.Add(localActions);
}
void InputManager::ClearBinds() {
    actions.ClearAll();
}


void InputManager::Do() {
    KeyCode key = getKeyCode();
    if (actions.contains(key)) {
        actions[key]();
    }
}