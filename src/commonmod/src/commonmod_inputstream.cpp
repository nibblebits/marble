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

#include "commonmod_inputstream.h"
#include "function.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>

CommonModule_InputStream::CommonModule_InputStream(Class* c) : Object(c)
{
    this->amount_to_fill = 512;
}

CommonModule_InputStream::~CommonModule_InputStream()
{

}

std::shared_ptr<Object> CommonModule_InputStream::newInstance(Class* c)
{
    return std::shared_ptr<CommonModule_InputStream>(new CommonModule_InputStream(c));
}


Class* CommonModule_InputStream::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * pure class InputStream
     * 
     * To be extended by all those wishing to implement their own InputStream functionality
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("InputStream");
    c->setDescriptorObject(std::make_shared<CommonModule_InputStream>(c));
    c->is_pure = true;
    /**
     * @class InputStream
     * Reads one byte from this InputStream's buffer
     * function read() : number
     */
    Function* f = c->registerFunction("read", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        InputStream_Read(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class InputStream
     * Writes one byte to this InputStream's buffer so that it can be read at a later time
     * function put(number n) : void
     */
    f = c->registerFunction("put", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        InputStream_Put(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class InputStream
     * To be implemented by input streams. The input streams should load the provided amount of bytes from their service into this input stream.
     * In short this method should fill the input stream with the given amount of bytes
     * pure function fill(number amount) : void
     */
    f = c->registerFunction("fill", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        InputStream_Fill(interpreter, arguments, return_value, object, caller_scope);
    });
    f->is_pure = true;
    f->access = MODIFIER_ACCESS_PROTECTED;

}

void CommonModule_InputStream::InputStream_Read(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_InputStream> stream = std::dynamic_pointer_cast<CommonModule_InputStream>(object);
    if (stream->buffer.empty())
    {
        // The stream buffer is empty so lets call the fill function to attempt to increase the stream buffer
        Function* function = stream->getClass()->getFunctionByNameAndArguments("fill", {VarType::fromString("number")});
        Value v;
        v.set((double)stream->amount_to_fill);
        function->invoke(interpreter, {v}, return_value, object, caller_scope);
    }

    if (stream->buffer.empty())
    {
        // We are still empty so lets throw an exception
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("EndOfStreamException"))),"", interpreter->getStackTraceLog());
    }

    // Let's now attempt to read from the input stream
    char c = stream->buffer.front();
    stream->buffer.pop_front();

    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = (unsigned char) c;
}

void CommonModule_InputStream::InputStream_Put(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Let's put the byte into the buffer
    std::shared_ptr<CommonModule_InputStream> stream = std::dynamic_pointer_cast<CommonModule_InputStream>(object);
    stream->buffer.push_back((char)arguments[0].dvalue);
}
void CommonModule_InputStream::InputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // We do nothing here, this marble function must be overrided by the class implementor
}