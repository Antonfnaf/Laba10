#pragma once
#include <cstring>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "Core/enums.h"

struct InputEvent {
    int true_code;   // Истинный код клавиши (если известен)
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
    static struct termios old_tio;
    static int support_level;

public:
    InputHandler();

    ~InputHandler();

    int kbhit();

    int getch();

    void CheckSupport();
        
    int GetSupportLevel() const {
        return support_level;
    }

    InputEvent GetInputEvent();
    
    KeyEvent GetKeyEvent(InputEvent event);
};