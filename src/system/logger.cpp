/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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

void Logger::warn(std::string message)
{
    PosInfo posInfo;
    warn(message, posInfo);
}

void Logger::error(std::string message)
{
    PosInfo posInfo;
    error(message, posInfo);
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
