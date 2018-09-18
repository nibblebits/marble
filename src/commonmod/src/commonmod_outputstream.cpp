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

#include "commonmod_outputstream.h"
#include "function.h"
#include "object.h"
#include "config.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>
CommonModule_OutputStream::CommonModule_OutputStream(Class* c) : Object(c)
{
    this->amount_to_auto_flush = MAX_OUTPUTSTREAM_SIZE_UNTIL_AUTO_FLUSH;
}

CommonModule_OutputStream::~CommonModule_OutputStream()
{
 
}

std::shared_ptr<Object> CommonModule_OutputStream::newInstance(Class* c)
{
    return std::make_shared<CommonModule_OutputStream>(c);
}

Class* CommonModule_OutputStream::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * pure class OutputStream
     * 
     * The OutputStream is a pure class that represents an output stream. This class provides functionality to write, get, empty and print to a output stream
     * You must extend this class if you are creating your own implementation of OutputStream. All output streams extend this class
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("OutputStream");
    c->setDescriptorObject(std::make_shared<CommonModule_OutputStream>(c));
    c->is_pure = true;
    /**
     * @class OutputStream
     * Writes one byte to the output stream buffer
     * 
     * function write(number n) : void
     */
    Function* f = c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Write(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class OutputStream
     * Flushes this output stream writing all data from the output stream buffer to the service this output stream is for.
     * This is a pure method and must be implemented by output streams
     * pure function flush() : void
     */
    f = c->registerFunction("flush", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Flush(interpreter, arguments, return_value, object, caller_scope);
    });
    // The flush function is pure
    f->is_pure = true;

    /**
     * @class OutputStream
     * Gets the byte at the given index in the output stream buffe
     * 
     * function get(number index) : number
     */
    f = c->registerFunction("get", {VarType::fromString("number")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Get(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class OutputStream
     * Clears the OutputStreams output stream buffer essentially making it as if no writes have happend before the last flush
     * function empty() : void
     */
    f = c->registerFunction("empty", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Empty(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class OutputStream
     * Returns the size of the OutputStream's output stream buffer since the last flush
     * function size() : number
     */
    f = c->registerFunction("size", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Size(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class OutputStream
     * Writes the given string to the OutputStream's output stream buffer
     * function print(string message) : void
     */
    f = c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Print(interpreter, arguments, return_value, object, caller_scope);
    });
}



void CommonModule_OutputStream::OutputStream_Write(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_OutputStream> output_stream = std::dynamic_pointer_cast<CommonModule_OutputStream>(object);
    output_stream->buffer.push_back((char)arguments[0].dvalue);
    if (output_stream->buffer.size() >= output_stream->amount_to_auto_flush)
    {
        Function* flush_func = object->getClass()->getFunctionByName("flush");
        flush_func->invoke(interpreter, {}, return_value, object, caller_scope);
    }      
}

void CommonModule_OutputStream::OutputStream_Get(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_OutputStream> output_stream = std::dynamic_pointer_cast<CommonModule_OutputStream>(object);
    return_value->type = VALUE_TYPE_NUMBER;
    try
    {
        return_value->dvalue = output_stream->buffer.at(arguments[0].dvalue);
    }
    catch(...)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))),"", interpreter->getStackTraceLog());
    }
}

void CommonModule_OutputStream::OutputStream_Empty(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_OutputStream> output_stream = std::dynamic_pointer_cast<CommonModule_OutputStream>(object);
    output_stream->buffer.clear();   
}

void CommonModule_OutputStream::OutputStream_Size(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_OutputStream> output_stream = std::dynamic_pointer_cast<CommonModule_OutputStream>(object);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = output_stream->buffer.size();
}

void CommonModule_OutputStream::OutputStream_Flush(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // This is pure within the marble language so we don't need to do anything here
}

void CommonModule_OutputStream::OutputStream_Print(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    Function* write_function = object->getClass()->getFunctionByNameAndArguments("write", {VarType::fromString("number")});
    std::string value = arguments[0].svalue;
    for (int i = 0; i < value.size(); i++)
    {
        Value v;
        v.type = VALUE_TYPE_NUMBER;
        v.set((double) value[i]);
        write_function->invoke(interpreter, {v}, NULL, object, caller_scope);
    }
}