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
    Class* c = moduleSystem->getClassSystem()->registerClass("OutputStream");
    c->setDescriptorObject(std::make_shared<CommonModule_OutputStream>(c));
    Function* f = c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Write(interpreter, arguments, return_value, object, caller_scope);
    });

    f = c->registerFunction("flush", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Flush(interpreter, arguments, return_value, object, caller_scope);
    });
    // The flush function is pure
    f->is_pure = true;

    f = c->registerFunction("get", {VarType::fromString("number")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Get(interpreter, arguments, return_value, object, caller_scope);
    });

    f = c->registerFunction("empty", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Empty(interpreter, arguments, return_value, object, caller_scope);
    });

    f = c->registerFunction("size", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        OutputStream_Size(interpreter, arguments, return_value, object, caller_scope);
    });

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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))));
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