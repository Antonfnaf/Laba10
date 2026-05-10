#pragma once 

#include "Core/enums.h"


struct LogMessage {
	std::string message;
	LogLevel lvl;
	LogType type;
	
	LogMessage(LogLevel lvl, LogType type, std::string message) : lvl(lvl), type(type), message(message){}
};
