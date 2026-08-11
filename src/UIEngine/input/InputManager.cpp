#include "UIEngine/input/InputManager.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include "UIEngine/input/InputHandler.hpp"
#endif


KeyCode InputManager::lastKeyCode;
KeyEvent InputManager::lastKeyEvent = { Key::None, 0, 0 };
InputEvent InputManager::lastRawEvent = {  0 , 0 };
Bind InputManager::actions;
InputHandler InputManager::handler;



KeyCode InputManager::GetNewKeyCode() {
#ifdef _WIN32
    // ============================================================
    // 1. ������� ��������� ������������ (�� ������ _getch!)
    // ============================================================
    bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

    // ��������� Caps Lock (��� ����)
    bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    // ============================================================
    // 2. ��������� ������� ������� � ������
    // ============================================================
    if (!_kbhit()) return KeyCode::None;

    int first = _getch();

    // ============================================================
    // 3. ������������ ������� (0x00 ��� 0xE0)
    // ============================================================
    if (first == 0x00 || first == 0xE0) {
        if (!_kbhit()) return KeyCode::None;
        int second = _getch();

        // ���������� ������� ������� ��� �������������
        KeyCode baseKey = KeyCode::None;

        switch (second) {
            // ������� � ���������
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

        // ��������� ������������
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
    // 4. CTRL + ����� (0x01-0x1A)
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
    // 5. ALT + ����� (��������� ����� GetAsyncKeyState)
    // ============================================================
    if (altPressed) {
        // ��������� ����� A-Z
        for (int i = 'A'; i <= 'Z'; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                return static_cast<KeyCode>(static_cast<int>(KeyCode::AltA) + (i - 'A'));
            }
        }
    }

    // ============================================================
    // 6. ������� ������� (0x20-0x7E)
    // ============================================================
    // ����� � ������ Shift � Caps Lock
    if (first >= 'A' && first <= 'Z') {
        bool upper = shiftPressed ^ capsLock;
        if (upper) {
            return static_cast<KeyCode>(first);  // ���������
        } else {
            return static_cast<KeyCode>(tolower(first));  // ��������
        }
    }

    // ��������� ������� (�����, ����� ����������, ������ � �.�.)
    // ���������� ��� ����, �.�. �������� ��������� � enum
    return static_cast<KeyCode>(first);
#else
// ============================================================
// LINUX IMPLEMENTATION (via InputHandler)
// ============================================================
    
    InputEvent rawEvent = handler.GetInputEvent();
    lastRawEvent = rawEvent; // Сохраняем последнее необработанное событие
    if (rawEvent.code == -1) {
        return KeyCode::None;
    }

    KeyEvent keyEvent = handler.GetKeyEvent(rawEvent);
    lastKeyEvent = keyEvent; // Сохраняем последнее событие клавиши
    if (keyEvent.key == Key::None) {
        return KeyCode::None;
    }

    uint8_t mods = static_cast<uint8_t>(keyEvent.modifiers);
    Key k = keyEvent.key;

    // --- ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ ДЛЯ БАЗОВОГО МАППИНГА ---
    // Преобразует Key в базовый KeyCode (без модификаторов)
    auto GetBaseCode = [](Key key) -> KeyCode {
        switch (key) {
            case Key::Space: return KeyCode::Space;
            case Key::Tab: return KeyCode::Tab;
            case Key::Enter: return KeyCode::Enter;
            case Key::Escape: return KeyCode::Escape;
            case Key::Delete: return KeyCode::Delete; // ASCII DEL
            
            // Буквы (в Key они строчные по значению, но мы маппим на строчные KeyCode)
            case Key::A: return KeyCode::a; case Key::B: return KeyCode::b;
            case Key::C: return KeyCode::c; case Key::D: return KeyCode::d;
            case Key::E: return KeyCode::e; case Key::F: return KeyCode::f;
            case Key::G: return KeyCode::g; case Key::H: return KeyCode::h;
            case Key::I: return KeyCode::i; case Key::J: return KeyCode::j;
            case Key::K: return KeyCode::k; case Key::L: return KeyCode::l;
            case Key::M: return KeyCode::m; case Key::N: return KeyCode::n;
            case Key::O: return KeyCode::o; case Key::P: return KeyCode::p;
            case Key::Q: return KeyCode::q; case Key::R: return KeyCode::r;
            case Key::S: return KeyCode::s; case Key::T: return KeyCode::t;
            case Key::U: return KeyCode::u; case Key::V: return KeyCode::v;
            case Key::W: return KeyCode::w; case Key::X: return KeyCode::x;
            case Key::Y: return KeyCode::y; case Key::Z: return KeyCode::z;

            // Цифры
            case Key::D0: return KeyCode::Digit0; case Key::D1: return KeyCode::Digit1;
            case Key::D2: return KeyCode::Digit2; case Key::D3: return KeyCode::Digit3;
            case Key::D4: return KeyCode::Digit4; case Key::D5: return KeyCode::Digit5;
            case Key::D6: return KeyCode::Digit6; case Key::D7: return KeyCode::Digit7;
            case Key::D8: return KeyCode::Digit8; case Key::D9: return KeyCode::Digit9;

            // Символы
            case Key::Exclamation: return KeyCode::Exclamation;
            case Key::DoubleQuote: return KeyCode::DoubleQuote;
            case Key::Hash: return KeyCode::Hash;
            case Key::Dollar: return KeyCode::Dollar;
            case Key::Percent: return KeyCode::Percent;
            case Key::Ampersand: return KeyCode::Ampersand;
            case Key::SingleQuote: return KeyCode::SingleQuote;
            case Key::LeftParen: return KeyCode::LeftParen;
            case Key::RightParen: return KeyCode::RightParen;
            case Key::Asterisk: return KeyCode::Asterisk;
            case Key::Plus: return KeyCode::Plus;
            case Key::Comma: return KeyCode::Comma;
            case Key::Minus: return KeyCode::Minus;
            case Key::Dot: return KeyCode::Dot;
            case Key::Slash: return KeyCode::Slash;
            case Key::Colon: return KeyCode::Colon;
            case Key::Semicolon: return KeyCode::Semicolon;
            case Key::Less: return KeyCode::Less;
            case Key::Equal: return KeyCode::Equal;
            case Key::Greater: return KeyCode::Greater;
            case Key::Question: return KeyCode::Question;
            case Key::At: return KeyCode::At;
            case Key::LeftBracket: return KeyCode::LeftBracket;
            case Key::Backslash: return KeyCode::Backslash;
            case Key::RightBracket: return KeyCode::RightBracket;
            case Key::Caret: return KeyCode::Caret;
            case Key::Underscore: return KeyCode::Underscore;
            case Key::Backtick: return KeyCode::Backtick;
            case Key::LeftBrace: return KeyCode::LeftBrace;
            case Key::Pipe: return KeyCode::Pipe;
            case Key::RightBrace: return KeyCode::RightBrace;
            case Key::Tilde: return KeyCode::Tilde;

            // Навигация и спец. клавиши
            case Key::UpArrow: return KeyCode::UpArrow;
            case Key::DownArrow: return KeyCode::DownArrow;
            case Key::LeftArrow: return KeyCode::LeftArrow;
            case Key::RightArrow: return KeyCode::RightArrow;
            case Key::Home: return KeyCode::Home;
            case Key::End: return KeyCode::End;
            case Key::PgUp: return KeyCode::PgUp;
            case Key::PgDn: return KeyCode::PgDn;
            case Key::Insert: return KeyCode::Insert;
            case Key::DeleteKey: return KeyCode::DeleteKey;

            // F-клавиши
            case Key::F1: return KeyCode::F1; case Key::F2: return KeyCode::F2;
            case Key::F3: return KeyCode::F3; case Key::F4: return KeyCode::F4;
            case Key::F5: return KeyCode::F5; case Key::F6: return KeyCode::F6;
            case Key::F7: return KeyCode::F7; case Key::F8: return KeyCode::F8;
            case Key::F9: return KeyCode::F9; case Key::F10: return KeyCode::F10;
            case Key::F11: return KeyCode::F11; case Key::F12: return KeyCode::F12;

            default: return KeyCode::None;
        }
    };

    KeyCode base = GetBaseCode(k);

    // --- ЛОГИКА МОДИФИКАТОРОВ ---

    // 1. Только Ctrl
    if (mods == static_cast<uint8_t>(Modifier::Ctrl)) {
        // Для букв A-Z возвращаем CtrlA...CtrlZ
        if (k >= Key::A && k <= Key::Z) {
            int offset = static_cast<int>(k) - static_cast<int>(Key::A);
            switch (offset) {
            case 7: return KeyCode::CtrlH;
            case 8: return KeyCode::CtrlI;
            case 12: return KeyCode::CtrlM; 
            default: return static_cast<KeyCode>(static_cast<int>(KeyCode::CtrlA) + offset);
            }
        }
        // Для специальных клавиш
        switch (k) {
            case Key::UpArrow: return KeyCode::CtrlUpArrow;
            case Key::DownArrow: return KeyCode::CtrlDownArrow;
            case Key::LeftArrow: return KeyCode::CtrlLeftArrow;
            case Key::RightArrow: return KeyCode::CtrlRightArrow;
            case Key::Home: return KeyCode::CtrlHome;
            case Key::End: return KeyCode::CtrlEnd;
            case Key::PgUp: return KeyCode::CtrlPgUp;
            case Key::PgDn: return KeyCode::CtrlPgDn;
            case Key::Insert: return KeyCode::CtrlInsert;
            case Key::DeleteKey: return KeyCode::CtrlDelete;
            case Key::F1: return KeyCode::CtrlF1; case Key::F2: return KeyCode::CtrlF2;
            case Key::F3: return KeyCode::CtrlF3; case Key::F4: return KeyCode::CtrlF4;
            case Key::F5: return KeyCode::CtrlF5; case Key::F6: return KeyCode::CtrlF6;
            case Key::F7: return KeyCode::CtrlF7; case Key::F8: return KeyCode::CtrlF8;
            case Key::F9: return KeyCode::CtrlF9; case Key::F10: return KeyCode::CtrlF10;
            case Key::F11: return KeyCode::CtrlF11; case Key::F12: return KeyCode::CtrlF12;
            default: break;
        }
    }

    // 2. Только Shift
    if (mods == static_cast<uint8_t>(Modifier::Shift)) {
        // Для букв возвращаем заглавные A...Z
        if (k >= Key::A && k <= Key::Z) {
            int offset = static_cast<int>(k) - static_cast<int>(Key::A);
            return static_cast<KeyCode>(static_cast<int>(KeyCode::A) + offset);
        }
        // Для навигации
        switch (k) {
            case Key::UpArrow: return KeyCode::ShiftUpArrow;
            case Key::DownArrow: return KeyCode::ShiftDownArrow;
            case Key::LeftArrow: return KeyCode::ShiftLeftArrow;
            case Key::RightArrow: return KeyCode::ShiftRightArrow;
            case Key::Home: return KeyCode::ShiftHome;
            case Key::End: return KeyCode::ShiftEnd;
            case Key::PgUp: return KeyCode::ShiftPgUp;
            case Key::PgDn: return KeyCode::ShiftPgDn;
            case Key::Insert: return KeyCode::ShiftInsert;
            case Key::DeleteKey: return KeyCode::ShiftDelete;
            case Key::F1: return KeyCode::ShiftF1; case Key::F2: return KeyCode::ShiftF2;
            case Key::F3: return KeyCode::ShiftF3; case Key::F4: return KeyCode::ShiftF4;
            case Key::F5: return KeyCode::ShiftF5; case Key::F6: return KeyCode::ShiftF6;
            case Key::F7: return KeyCode::ShiftF7; case Key::F8: return KeyCode::ShiftF8;
            case Key::F9: return KeyCode::ShiftF9; case Key::F10: return KeyCode::ShiftF10;
            case Key::F11: return KeyCode::ShiftF11; case Key::F12: return KeyCode::ShiftF12;
            default: break;
        }
    }

    // 3. Только Alt
    if (mods == static_cast<uint8_t>(Modifier::Alt)) {
        // Для букв возвращаем AltA...AltZ
        if (k >= Key::A && k <= Key::Z) {
            int offset = static_cast<int>(k) - static_cast<int>(Key::A);
            return static_cast<KeyCode>(static_cast<int>(KeyCode::AltA) + offset);
        }
        // Для навигации
        switch (k) {
            case Key::UpArrow: return KeyCode::AltUpArrow;
            case Key::DownArrow: return KeyCode::AltDownArrow;
            case Key::LeftArrow: return KeyCode::AltLeftArrow;
            case Key::RightArrow: return KeyCode::AltRightArrow;
            case Key::Home: return KeyCode::AltHome;
            case Key::End: return KeyCode::AltEnd;
            case Key::PgUp: return KeyCode::AltPgUp;
            case Key::PgDn: return KeyCode::AltPgDn;
            case Key::F1: return KeyCode::AltF1; case Key::F2: return KeyCode::AltF2;
            case Key::F3: return KeyCode::AltF3; case Key::F4: return KeyCode::AltF4;
            case Key::F5: return KeyCode::AltF5; case Key::F6: return KeyCode::AltF6;
            case Key::F7: return KeyCode::AltF7; case Key::F8: return KeyCode::AltF8;
            case Key::F9: return KeyCode::AltF9; case Key::F10: return KeyCode::AltF10;
            case Key::F11: return KeyCode::AltF11; case Key::F12: return KeyCode::AltF12;
            default: break;
        }
    }

    // 4. Комбинации модификаторов (Ctrl+Shift, Ctrl+Alt и т.д.)
    // Так как KeyCode не имеет отдельных значений для них, возвращаем базовый код.
    if (mods != 0) {
        return base;
    }

    // 5. Без модификаторов
    return base;
#endif
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
    Update();
    if (actions.contains(lastKeyCode)) {
        actions[lastKeyCode]();
    }
}