#include "logger.h"
#include <stdexcept>
LogEntry::LogEntry(const int level, const std::string message, const PosInfo posInfo) : level(level), message(message), posInfo(posInfo)
{
    
}
LogEntry::~LogEntry()
{

}

Logger::Logger()
{
    
}
Logger::~Logger()
{

}

void Logger::warn(std::string message, PosInfo posInfo)
{
    LogEntry entry(LOG_LEVEL_WARNING, message, posInfo);
    this->entries.push_back(entry);
}
void Logger::error(std::string message, PosInfo posInfo)
{
    LogEntry entry(LOG_LEVEL_ERROR, message, posInfo);
    this->entries.push_back(entry);
    throw std::logic_error(message);
}

bool Logger::hasLog()
{
    return !this->entries.empty();
}

bool Logger::hasErrors()
{
    for (LogEntry entry : this->entries)
    {
        if (entry.level == LOG_LEVEL_ERROR)
            return true;
    }
    
    return false;
}
