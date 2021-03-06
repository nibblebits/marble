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

#include <iostream>
#include "filemod.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "filemod_fileoutputstream.h"
FileModule_FileOutputStream::FileModule_FileOutputStream(Class* c) : CommonModule_OutputStream(c)
{
    this->file = NULL;
}

FileModule_FileOutputStream::~FileModule_FileOutputStream()
{

}

std::shared_ptr<Object> FileModule_FileOutputStream::newInstance(Class* c)
{
    return std::make_shared<FileModule_FileOutputStream>(c);
}

Class* FileModule_FileOutputStream::registerClass(ModuleSystem* moduleSystem)
{
    ClassSystem* c_system = moduleSystem->getClassSystem();
    /**
     * class FileOutputStream extends OutputStream
     * 
     * Responsible for outputting data to files. You can get a FileOutputStream object from a File once you have opened it
     */
    Class* c = c_system->registerClass("FileOutputStream", c_system->getClassByName("OutputStream"));
    c->setDescriptorObject(std::make_shared<FileModule_FileOutputStream>(c));
    /**
     * @class FileOutputStream
     * 
     * Flushes the output stream buffer to the file effectively writing the contents of the buffer to the file
     * function flush() : void
     */
    Function* f = c->registerFunction("flush", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        FileOutputStream_Flush(interpreter, arguments, return_value, object);
    });
    return c;
}

void FileModule_FileOutputStream::FileOutputStream_Flush(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_FileOutputStream> stream = std::dynamic_pointer_cast<FileModule_FileOutputStream>(object);
    // If this buffer is empty then we have nothing to flush so just return.
    if (stream->buffer.empty())
        return;

    /* If this output stream does not belong to a file then there is nothing we can do so throw an exception. 
     * This will happen when a programmer creates a FileOutputStream manually within marble code*/
    if (stream->file == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "", interpreter->getStackTraceLog());
    
    // Let's get the FILE* and write this buffer
    FILE* file = stream->file;

    // If the file is not open then lets throw an exception
    if (!file)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "", interpreter->getStackTraceLog());

    const char* buf = &stream->buffer[0];
    size_t amount = fwrite(buf, 1, stream->buffer.size(), file);

    // Did we write all bytes succesfully?
    if (amount != stream->buffer.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "", interpreter->getStackTraceLog());
    
    // Now we must clear the output stream buffer
    stream->buffer.clear();
}