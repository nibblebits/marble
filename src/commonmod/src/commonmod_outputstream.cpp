#include "commonmod_outputstream.h"
#include "function.h"
CommonModule_OutputStream::CommonModule_OutputStream(Class* c) : Object(c)
{
  
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
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<CommonModule_OutputStream>(c));
    Function* f = c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
    });
    f->is_pure = true;

    f = c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        Function* write_function = object->getClass()->getFunctionByNameAndArguments("write", {VarType::fromString("number")});
        std::string value = arguments[0].svalue;
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.type = VALUE_TYPE_NUMBER;
            v.set((double) value[i]);
            write_function->invoke(interpreter, {v}, NULL, object);
        }
    });
}

