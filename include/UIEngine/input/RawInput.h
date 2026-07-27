#pragma once
#include <vector>
#include <string>
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#include "Core/enums.h"

class RawInput {
private:
    std::string _prevStdin; // Буфер для фрагментированных escape-последовательностей

#ifdef _WIN32
    unsigned long _originalConsoleMode;
#else
    struct termios _originalTermios;
#endif

    // Внутренняя структура для результата парсинга одного символа/последовательности
    struct ParseResult {
        std::vector<Key> keys;
        size_t consumed;
        bool isComplete;
    };

    // Приватные методы парсинга
    std::string ReadRawBuffer();
    ParseResult TryParseSequence(const std::string& buffer, size_t startPos);
    ParseResult ParseCSI(const std::string& buffer, size_t startPos);
    ParseResult ParseSS3(const std::string& buffer, size_t startPos);

public:
    // Статические методы для управления режимом терминала
    void EnableRawMode();
    void DisableRawMode();

    // Основной метод получения клавиш за текущий тик
    std::vector<Key> GetKeyCodes();
};