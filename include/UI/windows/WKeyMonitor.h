#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "Core/enums.h"
#include "Core/Utils.h"
#include "UIEngine/composition/IWindow.h"
#include "UIEngine/input/InputManager.h"
// Маппинг для Key (используется в InputHandler)
const std::unordered_map<Key, std::string> KeyToString = {
    {Key::None, "None"},
    {Key::Space, "Space"}, {Key::Exclamation, "!"}, {Key::DoubleQuote, "\""}, {Key::Hash, "#"},
    {Key::Dollar, "$"}, {Key::Percent, "%"}, {Key::Ampersand, "&"}, {Key::SingleQuote, "'"},
    {Key::LeftParen, "("}, {Key::RightParen, ")"}, {Key::Asterisk, "*"}, {Key::Plus, "+"},
    {Key::Comma, ","}, {Key::Minus, "-"}, {Key::Dot, "."}, {Key::Slash, "/"},
    {Key::D0, "0"}, {Key::D1, "1"}, {Key::D2, "2"}, {Key::D3, "3"}, {Key::D4, "4"}, 
    {Key::D5, "5"}, {Key::D6, "6"}, {Key::D7, "7"}, {Key::D8, "8"}, {Key::D9, "9"},
    {Key::Colon, ":"}, {Key::Semicolon, ";"}, {Key::Less, "<"}, {Key::Equal, "="},
    {Key::Greater, ">"}, {Key::Question, "?"}, {Key::At, "@"},
    {Key::LeftBracket, "["}, {Key::Backslash, "\\"}, {Key::RightBracket, "]"},
    {Key::Caret, "^"}, {Key::Underscore, "_"}, {Key::Backtick, "`"},
    {Key::A, "a"}, {Key::B, "b"}, {Key::C, "c"}, {Key::D, "d"}, {Key::E, "e"}, {Key::F, "f"}, 
    {Key::G, "g"}, {Key::H, "h"}, {Key::I, "i"}, {Key::J, "j"}, {Key::K, "k"}, {Key::L, "l"}, {Key::M, "m"},
    {Key::N, "n"}, {Key::O, "o"}, {Key::P, "p"}, {Key::Q, "q"}, {Key::R, "r"}, {Key::S, "s"}, 
    {Key::T, "t"}, {Key::U, "u"}, {Key::V, "v"}, {Key::W, "w"}, {Key::X, "x"}, {Key::Y, "y"}, {Key::Z, "z"},
    {Key::LeftBrace, "{"}, {Key::Pipe, "|"}, {Key::RightBrace, "}"}, {Key::Tilde, "~"}, 
    {Key::Delete, "Backspace"},
    {Key::Tab, "Tab"}, {Key::Escape, "Esc"}, {Key::Enter, "Enter"},
    {Key::UpArrow, "Up"}, {Key::DownArrow, "Down"}, {Key::LeftArrow, "Left"}, {Key::RightArrow, "Right"},
    {Key::Home, "Home"}, {Key::End, "End"}, {Key::PgUp, "PgUp"}, {Key::PgDn, "PgDn"},
    {Key::Insert, "Ins"}, {Key::DeleteKey, "Del"},
    {Key::F1, "F1"}, {Key::F2, "F2"}, {Key::F3, "F3"}, {Key::F4, "F4"}, {Key::F5, "F5"}, {Key::F6, "F6"},
    {Key::F7, "F7"}, {Key::F8, "F8"}, {Key::F9, "F9"}, {Key::F10, "F10"}, {Key::F11, "F11"}, {Key::F12, "F12"}
};

// Маппинг для KeyCode (более подробный, включая Ctrl/Alt модификации)
const std::unordered_map<KeyCode, std::string> KeyCodeToString = {
    {KeyCode::None, "None"},
    // Управляющие
    {KeyCode::Null, "Null"}, {KeyCode::CtrlA, "Ctrl+A"}, {KeyCode::CtrlB, "Ctrl+B"}, {KeyCode::CtrlC, "Ctrl+C"},
    {KeyCode::CtrlD, "Ctrl+D"}, {KeyCode::CtrlE, "Ctrl+E"}, {KeyCode::CtrlF, "Ctrl+F"}, {KeyCode::CtrlG, "Ctrl+G"},
    {KeyCode::Backspace, "Backspace"}, {KeyCode::Tab, "Tab"}, {KeyCode::CtrlJ, "Ctrl+J"}, {KeyCode::CtrlK, "Ctrl+K"},
    {KeyCode::CtrlL, "Ctrl+L"}, {KeyCode::Enter, "Enter"}, {KeyCode::CtrlN, "Ctrl+N"}, {KeyCode::CtrlO, "Ctrl+O"},
    {KeyCode::CtrlP, "Ctrl+P"}, {KeyCode::CtrlQ, "Ctrl+Q"}, {KeyCode::CtrlR, "Ctrl+R"}, {KeyCode::CtrlS, "Ctrl+S"},
    {KeyCode::CtrlT, "Ctrl+T"}, {KeyCode::CtrlU, "Ctrl+U"}, {KeyCode::CtrlV, "Ctrl+V"}, {KeyCode::CtrlW, "Ctrl+W"},
    {KeyCode::CtrlX, "Ctrl+X"}, {KeyCode::CtrlY, "Ctrl+Y"}, {KeyCode::CtrlZ, "Ctrl+Z"},
    {KeyCode::Escape, "Esc"}, {KeyCode::CtrlBackslash, "Ctrl+\\"}, {KeyCode::CtrlRightBracket, "Ctrl+]"},
    {KeyCode::CtrlCaret, "Ctrl+^"}, {KeyCode::CtrlUnderscore, "Ctrl+_"},
    
    // Резервные Ctrl
    {KeyCode::CtrlH, "Ctrl+H(Res)"}, {KeyCode::CtrlI, "Ctrl+I(Res)"}, {KeyCode::CtrlM, "Ctrl+M(Res)"},

    // Alt буквы
    {KeyCode::AltA, "Alt+A"}, {KeyCode::AltB, "Alt+B"}, {KeyCode::AltC, "Alt+C"}, {KeyCode::AltD, "Alt+D"},
    {KeyCode::AltE, "Alt+E"}, {KeyCode::AltF, "Alt+F"}, {KeyCode::AltG, "Alt+G"}, {KeyCode::AltH, "Alt+H"},
    {KeyCode::AltI, "Alt+I"}, {KeyCode::AltJ, "Alt+J"}, {KeyCode::AltK, "Alt+K"}, {KeyCode::AltL, "Alt+L"},
    {KeyCode::AltM, "Alt+M"}, {KeyCode::AltN, "Alt+N"}, {KeyCode::AltO, "Alt+O"}, {KeyCode::AltP, "Alt+P"},
    {KeyCode::AltQ, "Alt+Q"}, {KeyCode::AltR, "Alt+R"}, {KeyCode::AltS, "Alt+S"}, {KeyCode::AltT, "Alt+T"},
    {KeyCode::AltU, "Alt+U"}, {KeyCode::AltV, "Alt+V"}, {KeyCode::AltW, "Alt+W"}, {KeyCode::AltX, "Alt+X"},
    {KeyCode::AltY, "Alt+Y"}, {KeyCode::AltZ, "Alt+Z"},

    // ASCII Печатные
    {KeyCode::Space, "Space"}, {KeyCode::Exclamation, "!"}, {KeyCode::DoubleQuote, "\""}, {KeyCode::Hash, "#"},
    {KeyCode::Dollar, "$"}, {KeyCode::Percent, "%"}, {KeyCode::Ampersand, "&"}, {KeyCode::SingleQuote, "'"},
    {KeyCode::LeftParen, "("}, {KeyCode::RightParen, ")"}, {KeyCode::Asterisk, "*"}, {KeyCode::Plus, "+"},
    {KeyCode::Comma, ","}, {KeyCode::Minus, "-"}, {KeyCode::Dot, "."}, {KeyCode::Slash, "/"},
    {KeyCode::Digit0, "0"}, {KeyCode::Digit1, "1"}, {KeyCode::Digit2, "2"}, {KeyCode::Digit3, "3"},
    {KeyCode::Digit4, "4"}, {KeyCode::Digit5, "5"}, {KeyCode::Digit6, "6"}, {KeyCode::Digit7, "7"},
    {KeyCode::Digit8, "8"}, {KeyCode::Digit9, "9"},
    {KeyCode::Colon, ":"}, {KeyCode::Semicolon, ";"}, {KeyCode::Less, "<"}, {KeyCode::Equal, "="},
    {KeyCode::Greater, ">"}, {KeyCode::Question, "?"}, {KeyCode::At, "@"},
    {KeyCode::A, "A"}, {KeyCode::B, "B"}, {KeyCode::C, "C"}, {KeyCode::D, "D"}, {KeyCode::E, "E"}, {KeyCode::F, "F"},
    {KeyCode::G, "G"}, {KeyCode::H, "H"}, {KeyCode::I, "I"}, {KeyCode::J, "J"}, {KeyCode::K, "K"}, {KeyCode::L, "L"},
    {KeyCode::M, "M"}, {KeyCode::N, "N"}, {KeyCode::O, "O"}, {KeyCode::P, "P"}, {KeyCode::Q, "Q"}, {KeyCode::R, "R"},
    {KeyCode::S, "S"}, {KeyCode::T, "T"}, {KeyCode::U, "U"}, {KeyCode::V, "V"}, {KeyCode::W, "W"}, {KeyCode::X, "X"},
    {KeyCode::Y, "Y"}, {KeyCode::Z, "Z"},
    {KeyCode::LeftBracket, "["}, {KeyCode::Backslash, "\\"}, {KeyCode::RightBracket, "]"},
    {KeyCode::Caret, "^"}, {KeyCode::Underscore, "_"}, {KeyCode::Backtick, "`"},
    {KeyCode::a, "a"}, {KeyCode::b, "b"}, {KeyCode::c, "c"}, {KeyCode::d, "d"}, {KeyCode::e, "e"}, {KeyCode::f, "f"},
    {KeyCode::g, "g"}, {KeyCode::h, "h"}, {KeyCode::i, "i"}, {KeyCode::j, "j"}, {KeyCode::k, "k"}, {KeyCode::l, "l"},
    {KeyCode::m, "m"}, {KeyCode::n, "n"}, {KeyCode::o, "o"}, {KeyCode::p, "p"}, {KeyCode::q, "q"}, {KeyCode::r, "r"},
    {KeyCode::s, "s"}, {KeyCode::t, "t"}, {KeyCode::u, "u"}, {KeyCode::v, "v"}, {KeyCode::w, "w"}, {KeyCode::x, "x"},
    {KeyCode::y, "y"}, {KeyCode::z, "z"},
    {KeyCode::LeftBrace, "{"}, {KeyCode::Pipe, "|"}, {KeyCode::RightBrace, "}"}, {KeyCode::Tilde, "~"},
    {KeyCode::Delete, "Del"},

    // Curses Extended
    {KeyCode::Home, "Home"}, {KeyCode::End, "End"}, {KeyCode::UpArrow, "Up"}, {KeyCode::DownArrow, "Down"},
    {KeyCode::LeftArrow, "Left"}, {KeyCode::RightArrow, "Right"}, {KeyCode::PgUp, "PgUp"}, {KeyCode::PgDn, "PgDn"},
    {KeyCode::Insert, "Ins"}, {KeyCode::DeleteKey, "DelKey"},
    {KeyCode::F1, "F1"}, {KeyCode::F2, "F2"}, {KeyCode::F3, "F3"}, {KeyCode::F4, "F4"}, {KeyCode::F5, "F5"},
    {KeyCode::F6, "F6"}, {KeyCode::F7, "F7"}, {KeyCode::F8, "F8"}, {KeyCode::F9, "F9"}, {KeyCode::F10, "F10"},
    {KeyCode::F11, "F11"}, {KeyCode::F12, "F12"},
    {KeyCode::KeyBackspaceCurses, "CursesBS"}, {KeyCode::KeyEnterCurses, "CursesEnter"}, {KeyCode::KeyTabBackward, "Shift+Tab"},

    // Модифицированные навигация и F-клавиши
    {KeyCode::CtrlUpArrow, "Ctrl+Up"}, {KeyCode::CtrlDownArrow, "Ctrl+Down"}, {KeyCode::CtrlLeftArrow, "Ctrl+Left"},
    {KeyCode::CtrlRightArrow, "Ctrl+Right"}, {KeyCode::CtrlHome, "Ctrl+Home"}, {KeyCode::CtrlEnd, "Ctrl+End"},
    {KeyCode::CtrlPgUp, "Ctrl+PgUp"}, {KeyCode::CtrlPgDn, "Ctrl+PgDn"}, {KeyCode::CtrlInsert, "Ctrl+Ins"},
    {KeyCode::CtrlDelete, "Ctrl+Del"}, {KeyCode::CtrlTab, "Ctrl+Tab"},
    {KeyCode::CtrlF1, "Ctrl+F1"}, {KeyCode::CtrlF2, "Ctrl+F2"}, {KeyCode::CtrlF3, "Ctrl+F3"}, {KeyCode::CtrlF4, "Ctrl+F4"},
    {KeyCode::CtrlF5, "Ctrl+F5"}, {KeyCode::CtrlF6, "Ctrl+F6"}, {KeyCode::CtrlF7, "Ctrl+F7"}, {KeyCode::CtrlF8, "Ctrl+F8"},
    {KeyCode::CtrlF9, "Ctrl+F9"}, {KeyCode::CtrlF10, "Ctrl+F10"}, {KeyCode::CtrlF11, "Ctrl+F11"}, {KeyCode::CtrlF12, "Ctrl+F12"},
    
    {KeyCode::ShiftUpArrow, "Shift+Up"}, {KeyCode::ShiftDownArrow, "Shift+Down"}, {KeyCode::ShiftLeftArrow, "Shift+Left"},
    {KeyCode::ShiftRightArrow, "Shift+Right"}, {KeyCode::ShiftHome, "Shift+Home"}, {KeyCode::ShiftEnd, "Shift+End"},
    {KeyCode::ShiftPgUp, "Shift+PgUp"}, {KeyCode::ShiftPgDn, "Shift+PgDn"}, {KeyCode::ShiftInsert, "Shift+Ins"},
    {KeyCode::ShiftDelete, "Shift+Del"},
    {KeyCode::ShiftF1, "Shift+F1"}, {KeyCode::ShiftF2, "Shift+F2"}, {KeyCode::ShiftF3, "Shift+F3"}, {KeyCode::ShiftF4, "Shift+F4"},
    {KeyCode::ShiftF5, "Shift+F5"}, {KeyCode::ShiftF6, "Shift+F6"}, {KeyCode::ShiftF7, "Shift+F7"}, {KeyCode::ShiftF8, "Shift+F8"},
    {KeyCode::ShiftF9, "Shift+F9"}, {KeyCode::ShiftF10, "Shift+F10"}, {KeyCode::ShiftF11, "Shift+F11"}, {KeyCode::ShiftF12, "Shift+F12"},

    {KeyCode::AltUpArrow, "Alt+Up"}, {KeyCode::AltDownArrow, "Alt+Down"}, {KeyCode::AltLeftArrow, "Alt+Left"},
    {KeyCode::AltRightArrow, "Alt+Right"}, {KeyCode::AltHome, "Alt+Home"}, {KeyCode::AltEnd, "Alt+End"},
    {KeyCode::AltPgUp, "Alt+PgUp"}, {KeyCode::AltPgDn, "Alt+PgDn"}, {KeyCode::AltTab, "Alt+Tab"},
    {KeyCode::AltF1, "Alt+F1"}, {KeyCode::AltF2, "Alt+F2"}, {KeyCode::AltF3, "Alt+F3"}, {KeyCode::AltF4, "Alt+F4"},
    {KeyCode::AltF5, "Alt+F5"}, {KeyCode::AltF6, "Alt+F6"}, {KeyCode::AltF7, "Alt+F7"}, {KeyCode::AltF8, "Alt+F8"},
    {KeyCode::AltF9, "Alt+F9"}, {KeyCode::AltF10, "Alt+F10"}, {KeyCode::AltF11, "Alt+F11"}, {KeyCode::AltF12, "Alt+F12"}
};

std::string bytes_to_hex_string(const std::array<uint8_t, 32>& seq, int length) {
    std::stringstream ss;
    for (int i = 0; i < length; ++i) {
        if (seq[i] < 0x21 || seq[i] > 0x7E) {
            ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(seq[i]);
        } else {
            ss << static_cast<char>(seq[i]);
        }
        
    }
    return ss.str();
}


class WKeyMonitor : public IWindow {
    mutable std::string prevStr = "";
    std::vector<std::vector<Pixel>> GetFrameLO(int width, int height) const {
        std::vector<std::vector<Pixel>> pic;
        
        std::string modsStr = std::to_string(InputManager::GetLastKeyEvent().modifiers);
        std::string keyStr = KeyToString.at(InputManager::GetLastKeyEvent().key);
        std::string keyCodeStr = KeyCodeToString.at(InputManager::GetLastKeyCode());
        std::string rawStr = bytes_to_hex_string(InputManager::GetLastRawEvent().raw_sequence, InputManager::GetLastRawEvent().raw_length);
        std::string fullStr = modsStr + " Key: " + keyStr + " | KeyCode: " + keyCodeStr + " | Raw: " + rawStr;
        if(!rawStr.empty()) {
            pic.push_back(Converter::ParseToPic(fullStr)[0]);
            prevStr = fullStr;
        } else {
            pic.push_back(Converter::ParseToPic(prevStr)[0]);
        }
        return pic;
    }
};