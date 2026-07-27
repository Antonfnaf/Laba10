#include "UIEngine/input/RawInput.h"
#include <algorithm>

void RawInput::EnableRawMode() {
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &_originalConsoleMode);
    SetConsoleMode(hIn, ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_WINDOW_INPUT);
#else
    tcgetattr(STDIN_FILENO, &_originalTermios);
    struct termios raw = _originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1; 
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
#endif
}

void RawInput::DisableRawMode() {
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, _originalConsoleMode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_originalTermios);
#endif
}

std::string RawInput::ReadRawBuffer() {
    std::string result;
#ifdef _WIN32
    while (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 0xE0) {
            result += (char)ch;
            result += (char)_getch();
        } else {
            result += (char)ch;
        }
    }
#else
    char buf[256];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n > 0) result.assign(buf, n);
#endif
    return result;
}

std::vector<Key> RawInput::GetKeyCodes() {
    std::string rawInput = ReadRawBuffer();
    std::string fullBuffer = _prevStdin + rawInput;
    _prevStdin.clear();

    std::vector<Key> result;
    size_t pos = 0;
    while (pos < fullBuffer.size()) {
        auto parseResult = TryParseSequence(fullBuffer, pos);
        if (parseResult.isComplete) {
            for (Key key : parseResult.keys) result.push_back(key);
            pos += parseResult.consumed;
        } else {
            _prevStdin = fullBuffer.substr(pos);
            break;
        }
    }
    return result;
}

RawInput::ParseResult RawInput::TryParseSequence(const std::string& buffer, size_t startPos) {
    if (startPos >= buffer.size()) return {{}, 0, true};
    char c = buffer[startPos];

    if (c == '\x1B') {
        if (startPos + 1 >= buffer.size()) return {{}, 0, false};
        char next = buffer[startPos + 1];
        if (next >= 0x20 && next <= 0x7E) {
            char lower = (next >= 'A' && next <= 'Z') ? (next + 0x20) : next;
            return {{Key::Alt, static_cast<Key>(lower)}, 2, true};
        }
        if (next == '[') return ParseCSI(buffer, startPos);
        if (next == 'O') return ParseSS3(buffer, startPos);
        return {{Key::Escape}, 1, true};
    }

    if (c >= 0x01 && c <= 0x1A) {
        if (c == 0x08) return {{Key::Backspace}, 1, true};
        if (c == 0x09) return {{Key::Tab}, 1, true};
        if (c == 0x0D) return {{Key::Enter}, 1, true};
        return {{Key::Ctrl, static_cast<Key>(c + 0x40)}, 1, true};
    }

    if (c >= 0x20 && c <= 0x7E) {
        if (c >= 'A' && c <= 'Z') {
            Key lower = static_cast<Key>(c + 0x20);
            return {{Key::Shift, lower}, 1, true};
        }
        // Маппинг символов с Shift+цифра (упрощенный)
        if (c == '!') return {{Key::Shift, Key::Digit1}, 1, true};
        if (c == '@') return {{Key::Shift, Key::Digit2}, 1, true};
        // ... можно добавить остальные по аналогии ...
        return {{static_cast<Key>(c)}, 1, true};
    }
    return {{Key::None}, 1, true};
}

RawInput::ParseResult RawInput::ParseCSI(const std::string& buffer, size_t startPos) {
    size_t endPos = startPos + 2;
    while (endPos < buffer.size()) {
        char c = buffer[endPos];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '~') break;
        endPos++;
    }
    if (endPos >= buffer.size()) return {{}, 0, false};

    std::string seq = buffer.substr(startPos, endPos - startPos + 1);
    if (seq == "\x1B[A") return {{Key::Up}, seq.length(), true};
    if (seq == "\x1B[B") return {{Key::Down}, seq.length(), true};
    if (seq == "\x1B[C") return {{Key::Right}, seq.length(), true};
    if (seq == "\x1B[D") return {{Key::Left}, seq.length(), true};
    if (seq == "\x1B[Z") return {{Key::Shift, Key::Tab}, seq.length(), true};
    
    // Обработка модификаторов для стрелок (например \x1B[1;5A)
    if (seq.find(';') != std::string::npos) {
        size_t semicolon = seq.find(';');
        size_t lastCharPos = seq.length() - 1;
        try {
            std::string modStr = seq.substr(semicolon + 1, lastCharPos - semicolon - 1);
            int mod = std::stoi(modStr);
            char finalChar = seq[lastCharPos];
            Key baseKey = Key::None;
            if (finalChar == 'A') baseKey = Key::Up;
            else if (finalChar == 'B') baseKey = Key::Down;
            else if (finalChar == 'C') baseKey = Key::Right;
            else if (finalChar == 'D') baseKey = Key::Left;

            if (baseKey != Key::None) {
                std::vector<Key> result;
                if (mod == 2) result = {Key::Shift, baseKey};
                if (mod == 5) result = {Key::Ctrl, baseKey};
                if (!result.empty()) return {result, seq.length(), true};
            }
        } catch (...) {}
    }
    return {{Key::Escape}, 1, true};
}

RawInput::ParseResult RawInput::ParseSS3(const std::string& buffer, size_t startPos) {
    if (startPos + 2 >= buffer.size()) return {{}, 0, false};
    char finalChar = buffer[startPos + 2];
    if (finalChar == 'P') return {{Key::F1}, 3, true};
    if (finalChar == 'Q') return {{Key::F2}, 3, true};
    if (finalChar == 'R') return {{Key::F3}, 3, true};
    if (finalChar == 'S') return {{Key::F4}, 3, true};
    return {{Key::Escape}, 1, true};
}