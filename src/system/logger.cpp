#include "logger.h"
#include <stdexcept>
LogEntry::LogEntry(int level, std::string message)
{
    this->level = level;
    this->message = message;
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

void Logger::error(std::string message)
{
    LogEntry entry(LOG_TYPE_ERROR, message);
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
        if (entry.level == LOG_TYPE_ERROR)
            return true;
    }
    
    return false;
}
