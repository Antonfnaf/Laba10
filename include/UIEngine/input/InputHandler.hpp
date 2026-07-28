#pragma once
#include <cstring>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "Core/enums.h"
#include "UIEngine/input//Bind.h"


class InputHandler {
private:
    struct termios old_tio;
    int support_level = 0;
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
};