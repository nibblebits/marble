#ifndef LOGGER_H
#define LOGGER_H
#include "statics.h"
#include <string>
#include <vector>
class LogEntry
{
public:
    LogEntry(int level, std::string message);
    virtual ~LogEntry();
    int level;
    std::string message;
};
class Logger
{
public:
    Logger();
    virtual ~Logger();
    void error(std::string message);
    bool hasLog();
    bool hasErrors();
    std::vector<LogEntry> entries;
};
#endif
