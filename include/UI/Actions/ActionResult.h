#pragma once


#include "Core/enums.h"

#include "UI/Logging/LogMessage.h"


struct ActionResult {
	bool success;
	
	LogMessage message;

	
	static ActionResult Error(const std::string& msg, LogType type = LogType::Error, LogLevel lvl = LogLevel::Normal) {
		return {false, LogMessage(lvl,type, msg)}
	}
	static ActionResult Success(const std::string& msg, LogType type = LogType::Success, LogLevel lvl = LogLevel::Normal) {
		return {true, LogMessage(lvl,type, msg)}
	}
};