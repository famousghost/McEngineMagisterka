#pragma once
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define __FILENAME__ strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
#define FILE_INFO  __FILENAME__
#define LINE TOSTRING(__LINE__)

#define LOG(msg, type) McEngine::Logger::getInstance().log(FILE_INFO, LINE, msg, type) 
#define SET_LOG_TYPE(type) McEngine::Logger::getInstance().setLogType(type)
