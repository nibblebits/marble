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
