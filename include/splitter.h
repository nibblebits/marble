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

#ifndef SPLITTER_H
#define SPLITTER_H

#include <iostream>

struct split;
struct data_descriptor
{
    int start;
    int end;
    int size;
    bool finished;

    const char* data;

    struct split* split;
 
    data_descriptor();
    virtual void update();
};

struct output_data : public data_descriptor
{
    void update();
};

struct marble_code : public data_descriptor
{
    void update();
};

struct split
{
    split();
    struct output_data output;
    struct marble_code code;
    bool has_code;
    bool has_data;
    bool is_last;
};

class Logger;
class Splitter
{
public:
    Splitter();
    Splitter(Logger* logger, const char* filename);
    virtual ~Splitter();
    void setData(const char* data, int length);
    bool split(struct split* marble_code);
    void free();
private:
    bool isSafeRange(int position);
    int findPositionOfMemoryInData(int position, const char* memory, int memory_size);
    int getPositionOfNextMarbleTag(int position);
    int getPositionOfNextMarbleClosingTag(int current_pos);

    bool was_freed;
    const char* data;
    int length;
    Logger* logger;
    const char* filename;
    struct marble_code* previous;
};
#endif
