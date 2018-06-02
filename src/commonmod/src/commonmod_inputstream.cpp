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
    Class* c = moduleSystem->getClassSystem()->registerClass("InputStream");
    c->setDescriptorObject(std::make_shared<CommonModule_InputStream>(c));
    c->is_pure = true;

    c->setDescriptorObject(std::make_shared<CommonModule_InputStream>(c));
    Function* f = c->registerFunction("read", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        InputStream_Read(interpreter, arguments, return_value, object, caller_scope);
    });

    f = c->registerFunction("put", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        InputStream_Put(interpreter, arguments, return_value, object, caller_scope);
    });

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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("EndOfStreamException"))));
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