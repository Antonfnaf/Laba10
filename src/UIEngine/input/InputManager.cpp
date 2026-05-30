#include "UIEngine/input/InputManager.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <ncurses.h>//for linux, not implement
#endif



Bind InputManager::actions;


void InputManager::Init() {}//for linux

void InputManager::Shutdown() {}//for linux




//bool InputManager::initialized = false;
//void InputManager::initInput(){
//    if (!initialized) {
//#ifdef _WIN32
//        // Windows PDCurses
//        initscr();
//        // Сразу отключаем вывод на экран
//        leaveok(stdscr, TRUE);
//        scrollok(stdscr, FALSE);
//#else
//        // Linux - создаём отдельный поток для ввода
//        SCREEN* scr = newterm(NULL, fopen("/dev/tty", "w+"), stdin);
//#endif
//        cbreak();
//        noecho();
//        nodelay(stdscr, TRUE);
//        keypad(stdscr, TRUE);
//
//#ifndef _WIN32
//        // 6. (Только Linux/macOS) Разрешаем обработку 8-го бита (Meta/Alt).
//        //    Позволяет ncurses устанавливать флаг A_ALT при нажатии Alt+клавиша.
//        //    На Windows/PDCurses это не требуется (там Alt определяется нативно).
//        meta(stdscr, TRUE);
//
//        // 7. Убираем задержку распознавания ESC-последовательностей.
//        //    По умолчанию ncurses ждёт 1 сек, чтобы отличить одиночный Escape от Alt+Key.
//        //    В nodelay-режиме это ломает отзывчивость. Ставим 0 для мгновенной реакции.
//        setenv("ESCDELAY", "0", 1);
//#endif
//        // 8. Скрываем курсор. Опционально, но рекомендуется для TUI-приложений,
//        //    чтобы он не мешал отрисовке и не мигал при каждом getch().
//        curs_set(0);
//        initialized = true;
//    }
//}
//void InputManager::shutdownInput() {
//    curs_set(1); // Возвращаем курсор
//    endwin();    // Освобождаем ресурсы curses
//}
//KeyCode InputManager::getKeyCode() {
//    Init();
//    std::lock_guard<std::mutex> lock(queueMutex);
//    if (keyQueue.empty()) {
//        return KeyCode::None;
//    }
//
//    int raw = static_cast<int>(keyQueue.front());
//    keyQueue.pop();
//
//
//    if (raw == ERR) return KeyCode::None; // Ничего не нажато
//
//    bool alt = false, ctrl = false, shift = false;
//    // Извлекаем битовые флаги модификаторов
//#ifdef _WIN32
//// PDCurses возвращает состояние модификаторов отдельным вызовом
//    int mods = PDC_get_key_modifiers();
//    alt = (mods & PDC_KEY_MODIFIER_ALT) != 0;
//    ctrl = (mods & PDC_KEY_MODIFIER_CONTROL) != 0;
//    shift = (mods & PDC_KEY_MODIFIER_SHIFT) != 0;
//#else
//    // NCurses упаковывает модификаторы прямо в возвращаемое значение
//    alt = (raw & A_ALT) != 0;
//    ctrl = (raw & A_CTRL) != 0;
//    shift = (raw & A_SHIFT) != 0;
//#endif
//
//    // Отсекаем флаги, оставляя только "чистый" код символа/клавиши
//    int key = raw & A_CHARTEXT;
//
//    // =========================================================================
//    // БЛОК 1: Расширенные клавиши (стрелки, F-клавиши, Home, PgUp и т.д.)
//    // В curses их коды всегда > 127
//    // =========================================================================
//    if (key > 127) {
//        switch (key) {
//        case KEY_UP:    return alt ? KeyCode::AltUpArrow : ctrl ? KeyCode::CtrlUpArrow : shift ? KeyCode::ShiftUpArrow : KeyCode::UpArrow;
//        case KEY_DOWN:  return alt ? KeyCode::AltDownArrow : ctrl ? KeyCode::CtrlDownArrow : shift ? KeyCode::ShiftDownArrow : KeyCode::DownArrow;
//        case KEY_LEFT:  return alt ? KeyCode::AltLeftArrow : ctrl ? KeyCode::CtrlLeftArrow : shift ? KeyCode::ShiftLeftArrow : KeyCode::LeftArrow;
//        case KEY_RIGHT: return alt ? KeyCode::AltRightArrow : ctrl ? KeyCode::CtrlRightArrow : shift ? KeyCode::ShiftRightArrow : KeyCode::RightArrow;
//
//        case KEY_HOME:  return alt ? KeyCode::AltHome : ctrl ? KeyCode::CtrlHome : shift ? KeyCode::ShiftHome : KeyCode::Home;
//        case KEY_END:   return alt ? KeyCode::AltEnd : ctrl ? KeyCode::CtrlEnd : shift ? KeyCode::ShiftEnd : KeyCode::End;
//        case KEY_PPAGE: return alt ? KeyCode::AltPgUp : ctrl ? KeyCode::CtrlPgUp : shift ? KeyCode::ShiftPgUp : KeyCode::PgUp;
//        case KEY_NPAGE: return alt ? KeyCode::AltPgDn : ctrl ? KeyCode::CtrlPgDn : shift ? KeyCode::ShiftPgDn : KeyCode::PgDn;
//
//        case KEY_IC:    return ctrl ? KeyCode::CtrlInsert : shift ? KeyCode::ShiftInsert : KeyCode::Insert; // Alt+Insert нет в enum -> fallback на Insert
//        case KEY_DC:    return ctrl ? KeyCode::CtrlDelete : shift ? KeyCode::ShiftDelete : KeyCode::DeleteKey;
//
//            // F1-F12
//        case KEY_F(1):  return alt ? KeyCode::AltF1 : ctrl ? KeyCode::CtrlF1 : shift ? KeyCode::ShiftF1 : KeyCode::F1;
//        case KEY_F(2):  return alt ? KeyCode::AltF2 : ctrl ? KeyCode::CtrlF2 : shift ? KeyCode::ShiftF2 : KeyCode::F2;
//        case KEY_F(3):  return alt ? KeyCode::AltF3 : ctrl ? KeyCode::CtrlF3 : shift ? KeyCode::ShiftF3 : KeyCode::F3;
//        case KEY_F(4):  return alt ? KeyCode::AltF4 : ctrl ? KeyCode::CtrlF4 : shift ? KeyCode::ShiftF4 : KeyCode::F4;
//        case KEY_F(5):  return alt ? KeyCode::AltF5 : ctrl ? KeyCode::CtrlF5 : shift ? KeyCode::ShiftF5 : KeyCode::F5;
//        case KEY_F(6):  return alt ? KeyCode::AltF6 : ctrl ? KeyCode::CtrlF6 : shift ? KeyCode::ShiftF6 : KeyCode::F6;
//        case KEY_F(7):  return alt ? KeyCode::AltF7 : ctrl ? KeyCode::CtrlF7 : shift ? KeyCode::ShiftF7 : KeyCode::F7;
//        case KEY_F(8):  return alt ? KeyCode::AltF8 : ctrl ? KeyCode::CtrlF8 : shift ? KeyCode::ShiftF8 : KeyCode::F8;
//        case KEY_F(9):  return alt ? KeyCode::AltF9 : ctrl ? KeyCode::CtrlF9 : shift ? KeyCode::ShiftF9 : KeyCode::F9;
//        case KEY_F(10): return alt ? KeyCode::AltF10 : ctrl ? KeyCode::CtrlF10 : shift ? KeyCode::ShiftF10 : KeyCode::F10;
//        case KEY_F(11): return alt ? KeyCode::AltF11 : ctrl ? KeyCode::CtrlF11 : shift ? KeyCode::ShiftF11 : KeyCode::F11;
//        case KEY_F(12): return alt ? KeyCode::AltF12 : ctrl ? KeyCode::CtrlF12 : shift ? KeyCode::ShiftF12 : KeyCode::F12;
//
//        default: return static_cast<KeyCode>(key); // fallback для редких KEY_*
//        }
//    }
//
//    // =========================================================================
//    // БЛОК 2: Alt + Буквы (a-z) -> маппинг в AltA..AltZ
//    // =========================================================================
//    if (alt && key >= 'a' && key <= 'z') {
//        // Статическая таблица lookup. Компилятор развернёт её в эффективный код.
//        constexpr KeyCode altMap[] = {
//            KeyCode::AltA, KeyCode::AltB, KeyCode::AltC, KeyCode::AltD, KeyCode::AltE,
//            KeyCode::AltF, KeyCode::AltG, KeyCode::AltH, KeyCode::AltI, KeyCode::AltJ,
//            KeyCode::AltK, KeyCode::AltL, KeyCode::AltM, KeyCode::AltN, KeyCode::AltO,
//            KeyCode::AltP, KeyCode::AltQ, KeyCode::AltR, KeyCode::AltS, KeyCode::AltT,
//            KeyCode::AltU, KeyCode::AltV, KeyCode::AltW, KeyCode::AltX, KeyCode::AltY, KeyCode::AltZ
//        };
//        return altMap[key - 'a'];
//    }
//
//    // =========================================================================
//    // БЛОК 3: Стандартные ASCII символы и Ctrl+буквы
//    // Благодаря точному совпадению значений в вашем enum, простое приведение
//    // типа автоматически возвращает правильные KeyCode:
//    //   0x01-0x1A -> CtrlA..CtrlZ
//    //   0x08      -> Backspace (физически Ctrl+H)
//    //   0x09      -> Tab (физически Ctrl+I)
//    //   0x0D      -> Enter (физически Ctrl+M)
//    //   0x1B      -> Escape
//    //   0x20-0x7E -> Пробел, буквы, цифры, символы
//    // =========================================================================
//    return static_cast<KeyCode>(key);
//}

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