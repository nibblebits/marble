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

#ifndef FILEMOD_FILE
#define FILEMOD_FILE

#include <memory>
#include <string>
#include <stdio.h>
#include "filepermission.h"
#include "filemod_fileoutputstream.h"
#include "filemod_fileinputstream.h"
#include "object.h"
#include "module.h"

class Interpreter;
class Scope;
class FileModule_File : public Object
{
public:
    FileModule_File(Class* c);
    virtual ~FileModule_File();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);


    /**
     * Called when a new interpreter is running
     */
    static void newInterpreter(Interpreter* interpreter);

    FILE* fp;
    std::string filename;
    std::shared_ptr<FileModule_FileOutputStream> output;
    std::shared_ptr<FileModule_FileInputStream> input;

private:
    // Native File functions/methods
    static void File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_setPosition(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_GetSize(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_Move(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_IsFile(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_Delete(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_file_get_contents(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_file_put_contents(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_chmod(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif