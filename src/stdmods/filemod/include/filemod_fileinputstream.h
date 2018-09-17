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

#ifndef FILEMOD_FILEINPUTSTREAM_H
#define FILEMOD_FILEINPUTSTREAM_H
#include "module.h"
#include "../../../commonmod/include/commonmod.h"
#include <memory>

class FileModule_File;
class FileModule_FileInputStream : public CommonModule_InputStream
{
public:
    FileModule_FileInputStream(Class* c);
    virtual ~FileModule_FileInputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);
    FILE* file;

    // Native FileOutputStream methods

    static void FileInputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object);
};

#endif