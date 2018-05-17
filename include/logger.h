#ifndef LOGGER_H
#define LOGGER_H
#include "statics.h"
#include <string>
#include <vector>
#include "posinfo.h"
class LogEntry
{
public:
    LogEntry(int level, std::string message, PosInfo posInfo);
    virtual ~LogEntry();
    const int level;
    const std::string message;
    const PosInfo posInfo;
};
class Logger
{
public:
    Logger();
    virtual ~Logger();
    void warn(std::string message);
    void error(std::string message);
    void warn(std::string message, PosInfo posInfo);
    void error(std::string message, PosInfo posInfo);
    bool hasLog();
    bool hasErrors();
    std::vector<LogEntry> entries;
};
#endif
