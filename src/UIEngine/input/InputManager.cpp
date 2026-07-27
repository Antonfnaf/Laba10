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
KeyChord parseChar(unsigned char c) {
    // Ctrl+����� (0x01-0x1A) - ���������� ��� Ctrl + �����
    // �� ��� ����� ��� ����� ������� ��� �������: LeftCtrl � �����
    // ��� ��� ������� ���������� ���� �������, �� ����� ������������ ��� 
    // � ������� ������� getKeyChords()
    if (c >= 0x01 && c <= 0x1A) {
        // ��� Ctrl+�����, �� �� �� ����� ������� ��� ������� �� ����� �������
        // ������� � parseChar �� ���������� ����������� ��������,
        // � � getKeyChords ������������ ��� ��� Ctrl + �����
        return static_cast<KeyChord>(0xF000 + c);  // ��������� ������
    }

    // ����������� �������
    if (c == 0x1B) return KeyChord::Escape;
    if (c == 0x0D) return KeyChord::Enter;
    if (c == 0x09) return KeyChord::Tab;
    if (c == 0x08) return KeyChord::Backspace;
    if (c == 0x7F) return KeyChord::Delete;

    // �������� ������� (ASCII)
    if (c >= 0x20 && c <= 0x7E) {
        return static_cast<KeyChord>(c);
    }

    return KeyChord::None;
}
//std::vector<KeyChord> getKeyChords() {
//    std::vector<KeyChord> result;
//    std::unordered_set<KeyChord> unique_keys;
//
//    // ������ ��� ��������� ������ �� ������
//    char buffer[1024];
//    int bytes = 0;
//
//    while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
//        int pos = 0;
//        while (pos < bytes) {
//            unsigned char c = static_cast<unsigned char>(buffer[pos]);
//
//            // ��������� ESC-�������������������
//            if (c == 0x1B) {
//                std::string seq = parseEscapeSequence(buffer, bytes, pos);
//
//                // ���������, �� Alt+����� �� ��� (ESC + �����)
//                if (seq.size() == 2 && seq[0] == 0x1B) {
//                    char letter = seq[1];
//                    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z')) {
//                        // Alt+����� - ��������� ��� �������
//                        unique_keys.insert(KeyChord::LeftAlt);
//                        unique_keys.insert(static_cast<KeyChord>(letter));
//                    }
//                } else {
//                    KeyChord code = parseEscSequence(seq);
//                    if (code != KeyChord::None) {
//                        unique_keys.insert(code);
//                    }
//                }
//                pos += seq.length();
//            } else {
//                // ������� ������
//                if (c >= 0x01 && c <= 0x1A) {
//                    // Ctrl+����� - ��������� Ctrl � �����
//                    unique_keys.insert(KeyChord::LeftCtrl);
//
//                    // ����������� 0x01->'A', 0x02->'B', ..., 0x1A->'Z'
//                    char letter = 'A' + (c - 0x01);
//                    unique_keys.insert(static_cast<KeyChord>(letter));
//                } else {
//                    KeyChord code = parseChar(c);
//                    if (code != KeyChord::None) {
//                        unique_keys.insert(code);
//                    }
//                }
//                pos++;
//            }
//        }
//    }
//    
//    // ��������� ������������, ������� ������������
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


KeyChord InputManager::getKeyCode() {
    // ============================================================
    // 1. ������� ��������� ������������ (�� ������ _getch!)
    // ============================================================
    //bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    //bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    //bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

    // ��������� Caps Lock (��� ����)
    //bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    // ============================================================
    // 2. ��������� ������� ������� � ������
    // ============================================================
    //if (!_kbhit()) 
    return KeyChord::None;

    //int first = _getch();
//
    //// ============================================================
    //// 3. ������������ ������� (0x00 ��� 0xE0)
    //// ============================================================
    //if (first == 0x00 || first == 0xE0) {
    //    if (!_kbhit()) return KeyChord::None;
    //    int second = _getch();
//
    //    // ���������� ������� ������� ��� �������������
    //    KeyChord baseKey = KeyChord::None;
//
    //    switch (second) {
    //        // ������� � ���������
    //    case 0x48: baseKey = KeyChord::UpArrow;    break;    // Up
    //    case 0x8d: baseKey = KeyChord::CtrlUpArrow; break;    // Ctrl+Up
    //    case 0x98: baseKey = KeyChord::AltUpArrow; break;    // Alt+Up
    //               
    //    case 0x50: baseKey = KeyChord::DownArrow;  break;   // Down
    //    case 0x91: baseKey = KeyChord::CtrlDownArrow; break;  // Ctrl+Down
    //    case 0xA0: baseKey = KeyChord::AltDownArrow; break;  // Alt+Down
    //               
    //    case 0x4B: baseKey = KeyChord::LeftArrow;  break;    // Left
    //    case 0x73: baseKey = KeyChord::CtrlLeftArrow; break;  // Ctrl+Left
    //    case 0x9B: baseKey = KeyChord::AltLeftArrow; break;   // Alt+Left
    //               
    //    case 0x4D: baseKey = KeyChord::RightArrow; break;   // Right
    //    case 0x74: baseKey = KeyChord::CtrlRightArrow; break; // Ctrl+Right
    //    case 0x9D: baseKey = KeyChord::AltRightArrow; break; // Alt+Right
//
    //        // F1-F12
    //    case 0x3B: baseKey = KeyChord::F1; break;
    //    case 0x3C: baseKey = KeyChord::F2; break;
    //    case 0x3D: baseKey = KeyChord::F3; break;
    //    case 0x3E: baseKey = KeyChord::F4; break;
    //    case 0x3F: baseKey = KeyChord::F5; break;
    //    case 0x40: baseKey = KeyChord::F6; break;
    //    case 0x41: baseKey = KeyChord::F7; break;
    //    case 0x42: baseKey = KeyChord::F8; break;
    //    case 0x43: baseKey = KeyChord::F9; break;
    //    case 0x44: baseKey = KeyChord::F10; break;
    //    case 0x85: baseKey = KeyChord::F11; break;
    //    case 0x86: baseKey = KeyChord::F12; break;
//
    //    default: return KeyChord::None;
    //    }
//
    //    // ��������� ������������
    //    if (altPressed) {
    //        switch (baseKey) {
    //        case KeyChord::UpArrow: return KeyChord::AltUpArrow;
    //        case KeyChord::DownArrow: return KeyChord::AltDownArrow;
    //        case KeyChord::LeftArrow: return KeyChord::AltLeftArrow;
    //        case KeyChord::RightArrow: return KeyChord::AltRightArrow;
    //        case KeyChord::Home: return KeyChord::AltHome;
    //        case KeyChord::End: return KeyChord::AltEnd;
    //        case KeyChord::PgUp: return KeyChord::AltPgUp;
    //        case KeyChord::PgDn: return KeyChord::AltPgDn;
    //        case KeyChord::Insert: return KeyChord::AltTab;  // Alt+Insert = AltTab
    //        case KeyChord::DeleteKey: return KeyChord::AltTab;
    //        case KeyChord::F1: return KeyChord::AltF1;
    //        case KeyChord::F2: return KeyChord::AltF2;
    //        case KeyChord::F3: return KeyChord::AltF3;
    //        case KeyChord::F4: return KeyChord::AltF4;
    //        case KeyChord::F5: return KeyChord::AltF5;
    //        case KeyChord::F6: return KeyChord::AltF6;
    //        case KeyChord::F7: return KeyChord::AltF7;
    //        case KeyChord::F8: return KeyChord::AltF8;
    //        case KeyChord::F9: return KeyChord::AltF9;
    //        case KeyChord::F10: return KeyChord::AltF10;
    //        case KeyChord::F11: return KeyChord::AltF11;
    //        case KeyChord::F12: return KeyChord::AltF12;
    //        default: return baseKey;
    //        }
    //    }
//
    //    if (ctrlPressed) {
    //        switch (baseKey) {
    //        case KeyChord::UpArrow: return KeyChord::CtrlUpArrow;
    //        case KeyChord::DownArrow: return KeyChord::CtrlDownArrow;
    //        case KeyChord::LeftArrow: return KeyChord::CtrlLeftArrow;
    //        case KeyChord::RightArrow: return KeyChord::CtrlRightArrow;
    //        case KeyChord::Home: return KeyChord::CtrlHome;
    //        case KeyChord::End: return KeyChord::CtrlEnd;
    //        case KeyChord::PgUp: return KeyChord::CtrlPgUp;
    //        case KeyChord::PgDn: return KeyChord::CtrlPgDn;
    //        case KeyChord::Insert: return KeyChord::CtrlInsert;
    //        case KeyChord::DeleteKey: return KeyChord::CtrlDelete;
    //        case KeyChord::F1: return KeyChord::CtrlF1;
    //        case KeyChord::F2: return KeyChord::CtrlF2;
    //        case KeyChord::F3: return KeyChord::CtrlF3;
    //        case KeyChord::F4: return KeyChord::CtrlF4;
    //        case KeyChord::F5: return KeyChord::CtrlF5;
    //        case KeyChord::F6: return KeyChord::CtrlF6;
    //        case KeyChord::F7: return KeyChord::CtrlF7;
    //        case KeyChord::F8: return KeyChord::CtrlF8;
    //        case KeyChord::F9: return KeyChord::CtrlF9;
    //        case KeyChord::F10: return KeyChord::CtrlF10;
    //        case KeyChord::F11: return KeyChord::CtrlF11;
    //        case KeyChord::F12: return KeyChord::CtrlF12;
    //        default: return baseKey;
    //        }
    //    }
//
    //    if (shiftPressed) {
    //        switch (baseKey) {
    //        case KeyChord::UpArrow: return KeyChord::ShiftUpArrow;
    //        case KeyChord::DownArrow: return KeyChord::ShiftDownArrow;
    //        case KeyChord::LeftArrow: return KeyChord::ShiftLeftArrow;
    //        case KeyChord::RightArrow: return KeyChord::ShiftRightArrow;
    //        case KeyChord::Home: return KeyChord::ShiftHome;
    //        case KeyChord::End: return KeyChord::ShiftEnd;
    //        case KeyChord::PgUp: return KeyChord::ShiftPgUp;
    //        case KeyChord::PgDn: return KeyChord::ShiftPgDn;
    //        case KeyChord::Insert: return KeyChord::ShiftInsert;
    //        case KeyChord::DeleteKey: return KeyChord::ShiftDelete;
    //        case KeyChord::F1: return KeyChord::ShiftF1;
    //        case KeyChord::F2: return KeyChord::ShiftF2;
    //        case KeyChord::F3: return KeyChord::ShiftF3;
    //        case KeyChord::F4: return KeyChord::ShiftF4;
    //        case KeyChord::F5: return KeyChord::ShiftF5;
    //        case KeyChord::F6: return KeyChord::ShiftF6;
    //        case KeyChord::F7: return KeyChord::ShiftF7;
    //        case KeyChord::F8: return KeyChord::ShiftF8;
    //        case KeyChord::F9: return KeyChord::ShiftF9;
    //        case KeyChord::F10: return KeyChord::ShiftF10;
    //        case KeyChord::F11: return KeyChord::ShiftF11;
    //        case KeyChord::F12: return KeyChord::ShiftF12;
    //        default: return baseKey;
    //        }
    //    }
//
    //    return baseKey;
    //}

    // // ============================================================
    // // 4. CTRL + ����� (0x01-0x1A)
    // // ============================================================
    // if (first >= 0x01 && first <= 0x1A) {
    //     // Ctrl+A = 0x01 ... Ctrl+Z = 0x1A
    //     if (ctrlPressed)
    //         switch (first) {
    //         case 0x08: return KeyChord::CtrlH;
    //         case 0x09: return KeyChord::CtrlI;
    //         case 0x0D: return KeyChord::CtrlM;
    //         default: break;
    //         }
    //     return static_cast<KeyChord>(static_cast<int>(KeyChord::CtrlA) + (first - 1));
    // }
    // if (ctrlPressed && first == 67) {
    //     return KeyChord::CtrlV;
    // }

    // // ============================================================
    // // 5. ALT + ����� (��������� ����� GetAsyncKeyState)
    // // ============================================================
    // if (altPressed) {
    //     // ��������� ����� A-Z
    //     for (int i = 'A'; i <= 'Z'; i++) {
    //         //if (GetAsyncKeyState(i) & 0x8000) {
    //         //    return static_cast<KeyChord>(static_cast<int>(KeyChord::AltA) + (i - 'A'));
    //         //}
    //     }
    // }

    // // ============================================================
    // // 6. ������� ������� (0x20-0x7E)
    // // ============================================================
    // // ����� � ������ Shift � Caps Lock
    // if (first >= 'A' && first <= 'Z') {
    //     bool upper = shiftPressed ^ capsLock;
    //     if (upper) {
    //         return static_cast<KeyChord>(first);  // ���������
    //     } else {
    //         return static_cast<KeyChord>(tolower(first));  // ��������
    //     }
    // }

    // // ��������� ������� (�����, ����� ����������, ������ � �.�.)
    // // ���������� ��� ����, �.�. �������� ��������� � enum
    // return static_cast<KeyChord>(first);
}

void InputManager::Bind(KeyChord key, std::function<void()> action) {
    actions.Add(key,action);
}
void InputManager::Bind(std::map<KeyChord, std::function<void()>> localActions) {
    actions.Add(localActions);
}
void InputManager::ClearBinds() {
    actions.ClearAll();
}


void InputManager::Do() {
    KeyChord key = getKeyCode();
    if (actions.contains(key)) {
        actions[key]();
    }
}