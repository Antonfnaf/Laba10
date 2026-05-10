#pragma once
#include <cstdint>

//enum class KeyCode : uint16_t {
//    None = 0xFFFF,  // специальное значение для отсутствия клавиши
//
//    // ASCII управляющие символы (0x00-0x1F)
//    Null = 0x00, CtrlA = 0x01, CtrlB = 0x02, CtrlC = 0x03, CtrlD = 0x04, CtrlE = 0x05,
//    CtrlF = 0x06, CtrlG = 0x07, Backspace = 0x08, Tab = 0x09, CtrlJ = 0x0A, CtrlK = 0x0B,
//    CtrlL = 0x0C, Enter = 0x0D, CtrlN = 0x0E, CtrlO = 0x0F, CtrlP = 0x10, CtrlQ = 0x11,
//    CtrlR = 0x12, CtrlS = 0x13, CtrlT = 0x14, CtrlU = 0x15, CtrlV = 0x16, CtrlW = 0x17,
//    CtrlX = 0x18, CtrlY = 0x19, CtrlZ = 0x1A, Escape = 0x1B, CtrlBackslash = 0x1C,
//    CtrlRightBracket = 0x1D, CtrlCaret = 0x1E, CtrlUnderscore = 0x1F,
//
//    // Печатные символы ASCII (0x20-0x7E)
//    Space = 0x20, Exclamation = 0x21, DoubleQuote = 0x22, Hash = 0x23, Dollar = 0x24,
//    Percent = 0x25, Ampersand = 0x26, SingleQuote = 0x27, LeftParen = 0x28, RightParen = 0x29,
//    Asterisk = 0x2A, Plus = 0x2B, Comma = 0x2C, Minus = 0x2D, Dot = 0x2E, Slash = 0x2F,
//    Digit0 = 0x30, Digit1 = 0x31, Digit2 = 0x32, Digit3 = 0x33, Digit4 = 0x34, Digit5 = 0x35,
//    Digit6 = 0x36, Digit7 = 0x37, Digit8 = 0x38, Digit9 = 0x39, Colon = 0x3A, Semicolon = 0x3B,
//    Less = 0x3C, Equal = 0x3D, Greater = 0x3E, Question = 0x3F, At = 0x40,
//    A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47, H = 0x48,
//    I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F, P = 0x50,
//    Q = 0x51, R = 0x52, S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57, X = 0x58,
//    Y = 0x59, Z = 0x5A, 
//    LeftBracket = 0x5B, Backslash = 0x5C, RightBracket = 0x5D,
//    Caret = 0x5E, Underscore = 0x5F, Backtick = 0x60,
//    a = 0x61, b = 0x62, c = 0x63, d = 0x64, e = 0x65, f = 0x66, g = 0x67, h = 0x68,
//    i = 0x69, j = 0x6A, k = 0x6B, l = 0x6C, m = 0x6D, n = 0x6E, o = 0x6F, p = 0x70,
//    q = 0x71, r = 0x72, s = 0x73, t = 0x74, u = 0x75, v = 0x76, w = 0x77, x = 0x78,
//    y = 0x79, z = 0x7A, 
//    LeftBrace = 0x7B, Pipe = 0x7C, RightBrace = 0x7D, Tilde = 0x7E,
//    Delete = 0x7F,
//
//    // Расширенные клавиши (двухбайтовые, первый байт 0x00 или 0xE0)
//    // Группа 0x00 (второй байт)
//    CtrlRightArrow = 0x00E0, CtrlLeftArrow = 0x00E1, CtrlDel = 0x00E2, CtrlEnd = 0x00E3,
//    CtrlPgDn = 0x00E4, CtrlHome = 0x00E5, CtrlPgUp = 0x00E6,
//
//    // Группа 0xE0
//    F1 = 0xE03B, F2 = 0xE03C, F3 = 0xE03D, F4 = 0xE03E, F5 = 0xE03F, F6 = 0xE040,
//    F7 = 0xE041, F8 = 0xE042, F9 = 0xE043, F10 = 0xE044, F11 = 0xE085, F12 = 0xE086,
//    Home = 0xE047, UpArrow = 0xE048, PgUp = 0xE049, LeftArrow = 0xE04B, RightArrow = 0xE04D,
//    End = 0xE04F, DownArrow = 0xE050, PgDn = 0xE051, Insert = 0xE052, DeleteKey = 0xE053,
//    CtrlUpArrow = 0xE08D, CtrlDownArrow = 0xE091,
//    CtrlF1 = 0xE05E, CtrlF2 = 0xE05F, CtrlF3 = 0xE060, CtrlF4 = 0xE061, CtrlF5 = 0xE062,
//    CtrlF6 = 0xE063, CtrlF7 = 0xE064, CtrlF8 = 0xE065, CtrlF9 = 0xE066, CtrlF10 = 0xE067,
//    CtrlF11 = 0xE089, CtrlF12 = 0xE08A,
//    ShiftF1 = 0xE054, ShiftF2 = 0xE055, ShiftF3 = 0xE056, ShiftF4 = 0xE057, ShiftF5 = 0xE058,
//    ShiftF6 = 0xE059, ShiftF7 = 0xE05A, ShiftF8 = 0xE05B, ShiftF9 = 0xE05C, ShiftF10 = 0xE05D,
//    ShiftF11 = 0xE087, ShiftF12 = 0xE088,
//    AltF1 = 0xE068, AltF2 = 0xE069, AltF3 = 0xE06A, AltF4 = 0xE06B, AltF5 = 0xE06C,
//    AltF6 = 0xE06D, AltF7 = 0xE06E, AltF8 = 0xE06F, AltF9 = 0xE070, AltF10 = 0xE071,
//    AltF11 = 0xE08B, AltF12 = 0xE08C
//};
enum class KeyCode : uint16_t
{
    None = 0xFFFF,  // специальное значение для отсутствия клавиши

    // =========================================================================
    // ASCII 0x00-0x1F: Управляющие символы (значения сохранены как в conio)
    // =========================================================================
    Null = 0x00,
    CtrlA = 0x01, CtrlB = 0x02, CtrlC = 0x03, CtrlD = 0x04, CtrlE = 0x05,
    CtrlF = 0x06, CtrlG = 0x07,
    Backspace = 0x08,  /*(Ctrl+H)*/
    Tab = 0x09, /*(Ctrl+I)*/
    CtrlJ = 0x0A, CtrlK = 0x0B,
    CtrlL = 0x0C,
    Enter = 0x0D,  /*(Ctrl+M)*/
    CtrlN = 0x0E, CtrlO = 0x0F, CtrlP = 0x10, CtrlQ = 0x11,
    CtrlR = 0x12, CtrlS = 0x13, CtrlT = 0x14, CtrlU = 0x15,
    CtrlV = 0x16, CtrlW = 0x17, CtrlX = 0x18, CtrlY = 0x19,
    CtrlZ = 0x1A,
    Escape = 0x1B,
    CtrlBackslash = 0x1C, CtrlRightBracket = 0x1D, CtrlCaret = 0x1E, CtrlUnderscore = 0x1F,

    // =========================================================================
    // "Зарезервированные" коды для перекрытых Ctrl+букв (не возвращаются curses)
    // Диапазон 0xF000-0xF0FF — для внутренней логики, если когда-то понадобится
    // =========================================================================
    CtrlH = 0xF001,  // Резерв: не отличим от Backspace в терминале
    CtrlI = 0xF002,  // Резерв: не отличим от Tab в терминале
    CtrlM = 0xF003,  // Резерв: не отличим от Enter в терминале
    // =========================================================================
    // Alt + Letters (резерв 0xF010-0xF029, для внешней привязки действий)
    // Примечание: В терминалах Alt+h и Alt+H обычно приходят одинаково.
    // Одно значение покрывает оба регистра.
    // =========================================================================
    AltA = 0xF010, AltB = 0xF011, AltC = 0xF012, AltD = 0xF013, AltE = 0xF014,
    AltF = 0xF015, AltG = 0xF016, AltH = 0xF017, AltI = 0xF018, AltJ = 0xF019,
    AltK = 0xF01A, AltL = 0xF01B, AltM = 0xF01C, AltN = 0xF01D, AltO = 0xF01E,
    AltP = 0xF01F, AltQ = 0xF020, AltR = 0xF021, AltS = 0xF022, AltT = 0xF023,
    AltU = 0xF024, AltV = 0xF025, AltW = 0xF026, AltX = 0xF027, AltY = 0xF028, AltZ = 0xF029,

    // =========================================================================
    // ASCII 0x20-0x7E: Печатные символы (значения = ASCII, имена как в conio)
    // =========================================================================
    Space = 0x20, Exclamation = 0x21, DoubleQuote = 0x22, Hash = 0x23,
    Dollar = 0x24, Percent = 0x25, Ampersand = 0x26, SingleQuote = 0x27,
    LeftParen = 0x28, RightParen = 0x29, Asterisk = 0x2A, Plus = 0x2B,
    Comma = 0x2C, Minus = 0x2D, Dot = 0x2E, Slash = 0x2F,
    Digit0 = 0x30, Digit1 = 0x31, Digit2 = 0x32, Digit3 = 0x33,
    Digit4 = 0x34, Digit5 = 0x35, Digit6 = 0x36, Digit7 = 0x37,
    Digit8 = 0x38, Digit9 = 0x39,
    Colon = 0x3A, Semicolon = 0x3B, Less = 0x3C, Equal = 0x3D,
    Greater = 0x3E, Question = 0x3F, At = 0x40,
    A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46,
    G = 0x47, H = 0x48, I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C,
    M = 0x4D, N = 0x4E, O = 0x4F, P = 0x50, Q = 0x51, R = 0x52,
    S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57, X = 0x58,
    Y = 0x59, Z = 0x5A,
    LeftBracket = 0x5B, Backslash = 0x5C, RightBracket = 0x5D,
    Caret = 0x5E, Underscore = 0x5F, Backtick = 0x60,
    a = 0x61, b = 0x62, c = 0x63, d = 0x64, e = 0x65, f = 0x66,
    g = 0x67, h = 0x68, i = 0x69, j = 0x6A, k = 0x6B, l = 0x6C,
    m = 0x6D, n = 0x6E, o = 0x6F, p = 0x70, q = 0x71, r = 0x72,
    s = 0x73, t = 0x74, u = 0x75, v = 0x76, w = 0x77, x = 0x78,
    y = 0x79, z = 0x7A,
    LeftBrace = 0x7B, Pipe = 0x7C, RightBrace = 0x7D, Tilde = 0x7E,

    // =========================================================================
    // ASCII 0x7F: Delete
    // =========================================================================
    Delete = 0x7F,  // ASCII DEL

    // =========================================================================
    // CURSES EXTENDED KEYS (значения = KEY_* из curses, имена как в conio)
    // Диапазон 256-511 — стандартные KEY_* константы
    // =========================================================================

    // --- Навигация (стрелки, домашние клавиши) ---
    Home = 262,   // KEY_HOME
    End = 360,   // KEY_END
    UpArrow = 259,   // KEY_UP
    DownArrow = 258,   // KEY_DOWN
    LeftArrow = 260,   // KEY_LEFT
    RightArrow = 261,   // KEY_RIGHT
    PgUp = 339,   // KEY_PPAGE
    PgDn = 338,   // KEY_NPAGE
    Insert = 331,   // KEY_IC
    DeleteKey = 330,   // KEY_DC (отличается от ASCII Delete=0x7F)

    // --- Функциональные клавиши F1-F12 ---
    F1 = 264, F2 = 265, F3 = 266, F4 = 267,
    F5 = 268, F6 = 269, F7 = 270, F8 = 271,
    F9 = 272, F10 = 273, F11 = 274, F12 = 275,

    // --- Дополнительные KEY_* для полноты ---
    KeyBackspaceCurses = 263,   // KEY_BACKSPACE (отличается от ASCII Backspace=0x08)
    KeyEnterCurses = 343,   // KEY_ENTER (Enter с numeric keypad)
    KeyTabBackward = 353,   // KEY_BTAB (Shift+Tab)

    // =========================================================================
    // "Зарезервированные" коды для модифицированных клавиш
    // curses не возвращает уникальные коды для Ctrl/Alt+стрелки по умолчанию,
    // поэтому даём им значения в диапазоне 0xF100-0xF2FF для будущей логики
    // =========================================================================

    // --- Ctrl + навигация (резерв, не возвращаются стандартным curses) ---
    CtrlUpArrow = 0xF100, CtrlDownArrow = 0xF101,
    CtrlLeftArrow = 0xF102, CtrlRightArrow = 0xF103,
    CtrlHome = 0xF104, CtrlEnd = 0xF105,
    CtrlPgUp = 0xF106, CtrlPgDn = 0xF107,
    CtrlInsert = 0xF108, CtrlDelete = 0xF109,
    CtrlTab = 0xF10A,

    // --- Ctrl + F1-F12 (резерв) ---
    CtrlF1 = 0xF110, CtrlF2 = 0xF111, CtrlF3 = 0xF112, CtrlF4 = 0xF113,
    CtrlF5 = 0xF114, CtrlF6 = 0xF115, CtrlF7 = 0xF116, CtrlF8 = 0xF117,
    CtrlF9 = 0xF118, CtrlF10 = 0xF119, CtrlF11 = 0xF11A, CtrlF12 = 0xF11B,

    // --- Shift + навигация (некоторые есть в curses как KEY_S*, но для единообразия) ---
    ShiftUpArrow = 0xF120, ShiftDownArrow = 0xF121,
    ShiftLeftArrow = 0xF122, ShiftRightArrow = 0xF123,
    ShiftHome = 0xF124, ShiftEnd = 0xF125,
    ShiftPgUp = 0xF126, ShiftPgDn = 0xF127,
    ShiftInsert = 0xF128, ShiftDelete = 0xF129,

    // --- Shift + F1-F12 (резерв) ---
    ShiftF1 = 0xF130, ShiftF2 = 0xF131, ShiftF3 = 0xF132, ShiftF4 = 0xF133,
    ShiftF5 = 0xF134, ShiftF6 = 0xF135, ShiftF7 = 0xF136, ShiftF8 = 0xF137,
    ShiftF9 = 0xF138, ShiftF10 = 0xF139, ShiftF11 = 0xF13A, ShiftF12 = 0xF13B,

    // --- Alt + навигация (резерв) ---
    AltUpArrow = 0xF140, AltDownArrow = 0xF141,
    AltLeftArrow = 0xF142, AltRightArrow = 0xF143,
    AltHome = 0xF144, AltEnd = 0xF145,
    AltPgUp = 0xF146, AltPgDn = 0xF147,
    AltTab = 0xF148,

    // --- Alt + F1-F12 (резерв) ---
    AltF1 = 0xF150, AltF2 = 0xF151, AltF3 = 0xF152, AltF4 = 0xF153,
    AltF5 = 0xF154, AltF6 = 0xF155, AltF7 = 0xF156, AltF8 = 0xF157,
    AltF9 = 0xF158, AltF10 = 0xF159, AltF11 = 0xF15A, AltF12 = 0xF15B,

    // =========================================================================
    // Дополнительные резервные коды (если понадобятся)
    // Диапазон 0xF200-0xFEFF — свободен для будущих расширений
    // =========================================================================
    Reserved_0 = 0xF200, Reserved_1 = 0xF201, Reserved_2 = 0xF202,
    // ... можно добавить больше при необходимости ...
};


enum class Color {
    // Basic colors (0-7)
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7,

    // Bright colors (8-15)
    BrightBlack = 8,     // Dark Gray
    BrightRed = 9,
    BrightGreen = 10,
    BrightYellow = 11,
    BrightBlue = 12,
    BrightMagenta = 13,
    BrightCyan = 14,
    BrightWhite = 15,

    // 216 RGB colors (16-231)
    Red1 = 16,      Red2 = 17,      Red3 = 18,      Red4 = 19,      Red5 = 20,      Red6 = 21,
    Orange1 = 22,   Orange2 = 23,   Orange3 = 24,   Orange4 = 25,   Orange5 = 26,   Orange6 = 27,
    Yellow1 = 28,   Yellow2 = 29,   Yellow3 = 30,   Yellow4 = 31,   Yellow5 = 32,   Yellow6 = 33,
    Green1 = 34,    Green2 = 35,    Green3 = 36,    Green4 = 37,    Green5 = 38,    Green6 = 39,
    Green7 = 40,    Green8 = 41,    Green9 = 42,    Green10 = 43,   Green11 = 44,   Green12 = 45,
    Green13 = 46,   Green14 = 47,   Green15 = 48,   Green16 = 49,   Green17 = 50,   Green18 = 51,

    // Cyan shades
    Cyan1 = 52,     Cyan2 = 53,     Cyan3 = 54,     Cyan4 = 55,     Cyan5 = 56,     Cyan6 = 57,
    Blue1 = 58,     Blue2 = 59,     Blue3 = 60,     Blue4 = 61,     Blue5 = 62,     Blue6 = 63,
    Blue7 = 64,     Blue8 = 65,     Blue9 = 66,     Blue10 = 67,    Blue11 = 68,    Blue12 = 69,
    Blue13 = 70,    Blue14 = 71,    Blue15 = 72,    Blue16 = 73,    Blue17 = 74,    Blue18 = 75,
    Purple1 = 76,   Purple2 = 77,   Purple3 = 78,   Purple4 = 79,   Purple5 = 80,   Purple6 = 81,
    Magenta1 = 82,  Magenta2 = 83,  Magenta3 = 84,  Magenta4 = 85,  Magenta5 = 86,  Magenta6 = 87,

    // Mixed RGB colors (various combinations)
    Teal1 = 88,     Teal2 = 89,     Teal3 = 90,     Teal4 = 91,     Teal5 = 92,     Teal6 = 93,
    Lime1 = 94,     Lime2 = 95,     Lime3 = 96,     Lime4 = 97,     Lime5 = 98,     Lime6 = 99,
    Olive1 = 100,   Olive2 = 101,   Olive3 = 102,   Olive4 = 103,   Olive5 = 104,   Olive6 = 105,
    Navy1 = 106,    Navy2 = 107,    Navy3 = 108,    Navy4 = 109,    Navy5 = 110,    Navy6 = 111,
    Maroon1 = 112,  Maroon2 = 113,  Maroon3 = 114,  Maroon4 = 115,  Maroon5 = 116,  Maroon6 = 117,
    Violet1 = 118,  Violet2 = 119,  Violet3 = 120,  Violet4 = 121,  Violet5 = 122,  Violet6 = 123,

    Coral1 = 124,   Coral2 = 125,   Coral3 = 126,   Coral4 = 127,   Coral5 = 128,   Coral6 = 129,
    Turquoise1 = 130,   Turquoise2 = 131,   Turquoise3 = 132,   Turquoise4 = 133,   Turquoise5 = 134,   Turquoise6 = 135,
    Lavender1 = 136,    Lavender2 = 137,    Lavender3 = 138,    Lavender4 = 139,    Lavender5 = 140,    Lavender6 = 141,
    Peach1 = 142,   Peach2 = 143,   Peach3 = 144,   Peach4 = 145,   Peach5 = 146,   Peach6 = 147,
    Mint1 = 148,    Mint2 = 149,    Mint3 = 150,    Mint4 = 151,    Mint5 = 152,    Mint6 = 153,
    Rose1 = 154,    Rose2 = 155,    Rose3 = 156,    Rose4 = 157,    Rose5 = 158,    Rose6 = 159,

    Sky1 = 160,     Sky2 = 161,     Sky3 = 162,     Sky4 = 163,     Sky5 = 164,     Sky6 = 165,
    Salmon1 = 166,  Salmon2 = 167,  Salmon3 = 168,  Salmon4 = 169,  Salmon5 = 170,  Salmon6 = 171,
    Plum1 = 172,    Plum2 = 173,    Plum3 = 174,    Plum4 = 175,    Plum5 = 176,    Plum6 = 177,
    Khaki1 = 178,   Khaki2 = 179,   Khaki3 = 180,   Khaki4 = 181,   Khaki5 = 182,   Khaki6 = 183,
    Bronze1 = 184,  Bronze2 = 185,  Bronze3 = 186,  Bronze4 = 187,  Bronze5 = 188,  Bronze6 = 189,
    Indigo1 = 190,  Indigo2 = 191,  Indigo3 = 192,  Indigo4 = 193,  Indigo5 = 194,  Indigo6 = 195,

    Crimson1 = 196, Crimson2 = 197, Crimson3 = 198, Crimson4 = 199, Crimson5 = 200, Crimson6 = 201,
    Aqua1 = 202,    Aqua2 = 203,    Aqua3 = 204,    Aqua4 = 205,    Aqua5 = 206,    Aqua6 = 207,
    OliveGreen1 = 208,  OliveGreen2 = 209,  OliveGreen3 = 210,  OliveGreen4 = 211,  OliveGreen5 = 212,  OliveGreen6 = 213,
    SteelBlue1 = 214,   SteelBlue2 = 215,   SteelBlue3 = 216,   SteelBlue4 = 217,   SteelBlue5 = 218,   SteelBlue6 = 219,
    WarmGray1 = 220,    WarmGray2 = 221,    WarmGray3 = 222,    WarmGray4 = 223,    WarmGray5 = 224,    WarmGray6 = 225,
    CoolGray1 = 226,    CoolGray2 = 227,    CoolGray3 = 228,    CoolGray4 = 229,    CoolGray5 = 230,    CoolGray6 = 231,

    // Grayscale (232-255) - 24 colors
    Black0 = 232,   // Almost black
    Black1 = 233,
    Black2 = 234,
    Black3 = 235,
    Black4 = 236,
    Black5 = 237,
    Black6 = 238,
    Black7 = 239,
    Gray0 = 240,
    Gray1 = 241,
    Gray2 = 242,
    Gray3 = 243,
    Gray4 = 244,
    Gray5 = 245,
    Gray6 = 246,
    Gray7 = 247,
    White0 = 248,
    White1 = 249,
    White2 = 250,
    White3 = 251,
    White4 = 252,
    White5 = 253,
    White6 = 254,
    White7 = 255    // Almost white
};

//enum class ShipType {
//    lincor,
//    yacht,
//    creiser,
//    ledocol,
//    lodka
//};

//enum class Country {
//    USA,
//    USSR,
//    UK,
//    Japan,
//    Germany
//};

//enum class ShipStatus {
//    InPort,
//    Traveling,
//    Fixing,
//    Lost
//};
enum class Direction {
    Up,
    Right,
    Down,
    Left
};
enum class BorderType {
    Simple,
    Heavy,
    Double,
    Curcle,
    ASCII
};

enum class BorderPlace {
    LineH,
    LineV,
    CornerLU,
    CornerRU,
    CornerLD,
    CornerRD,
    Cross,
    CrossL,
    CrossR,
    CrossU,
    CrossD
};

enum class Slot {
    Main,
    Actions,
    Sidebar,
    Overlay,

};

enum class LogLevel {
    Normal,
    Debug, //����� ������ � ������
};

enum class LogType {
    Success, //������ ���������� �� �������� ������������
    Notification, //��������� ���������� 
    Info, //���� � ��� �� (�� ����������)
    Error,
    Warning
};

enum class MPlace{
    Center,
    Up,
    Right,
    Down,
    Left,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
    Free
};