#include "Logger.h"
#include <iostream>
#include <ctime> 

namespace McEngine
{

Logger::Logger()
{
    try
    {
        m_logFile.open(prepareLogFile());
        if (not m_logFile.is_open())
        {
            throw std::exception();
        }
    }
    catch(std::exception& ex)
    {
        std::cerr << "Cannot create log file" << ex.what() << std::endl;
    }
}

Logger::~Logger()
{
    m_logFile.close();
}

void Logger::log(const std::string& p_fileInfo,
                 const std::string& p_line,
                 const std::string& p_informationMessage, 
                 LogType p_logType)
{
   if (s_logType >= p_logType)
   {
       std::string l_logInformation =  "[" + getLogType(p_logType) + "][" + p_fileInfo + ":" + p_line + "]: " + p_informationMessage + "\n";
       m_logFile << l_logInformation;
   }
}

void Logger::setLogType(LogType p_logType)
{
    s_logType = p_logType;
}

std::string Logger::getCurrentTime()
{
    time_t l_rawtime;
    struct tm * l_timeinfo;
    char l_buffer[80];

    time(&l_rawtime);
    l_timeinfo = localtime(&l_rawtime);

    strftime(l_buffer, sizeof(l_buffer), "%d_%m_%Y_%H_%M_%S", l_timeinfo);
    std::string str(l_buffer);
    
    return str;
}

std::string Logger::prepareLogFile()
{
    return "Logs/LOG_" + getCurrentTime() + ".log";
}

Logger& Logger::getInstance()
{
    static Logger m_logger;
    return m_logger;
}

std::string Logger::getLogType(LogType p_logType)
{
    switch (p_logType)
    {
    case LogType::INF:
        return "INFO";
    case LogType::WARN:
        return "WARNING";
    case LogType::DBG:
        return "DEBUG";
    case LogType::ERR:
        return "ERROR";
    default:
        return "INFO";
    }
}

}//GameEngine
