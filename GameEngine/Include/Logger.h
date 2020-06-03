#pragma once
#include <string>
#include <fstream>
#include <string.h>
#include <string_view>
#include <exception>
#include "Defines.h"

namespace McEngine
{

enum class LogType : unsigned int
{
    INF = 0,
    WARN = 1,
    ERR = 2,
    DBG = 3,
};

class Logger : public std::exception
{
public:
    Logger();
    ~Logger();
    void log(const std::string& p_fileInfo,
                    const std::string& p_line,
                    const std::string& p_informationMessage,
                    LogType p_logType);

    void setLogType(LogType p_logType);

    const std::string& fetchConsoleLogs();

    static Logger& getInstance();
private:
    std::string prepareLogFile();
    std::string getCurrentTime();
    std::string getLogType(LogType p_logType);

    LogType s_logType;
    std::ofstream m_logFile;
    std::string m_logToConsole;
};

}//GameEngine

