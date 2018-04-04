#include "commonmod_sqldriver.h"
#include "modulesystem.h"
#include <iostream>
CommonModule_SqlDriver::CommonModule_SqlDriver(Class* c) : Object(c)
{

}

CommonModule_SqlDriver::~CommonModule_SqlDriver()
{

}

std::shared_ptr<Object> CommonModule_SqlDriver::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlDriver>(c);
}
    
Class* CommonModule_SqlDriver::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLDriver");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlDriver>(c));
    c->is_pure = true;

    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_Construct);

}


// Native SQLDriver functions
void CommonModule_SqlDriver::SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor
    object->getClass("SQLDriver")->invokeObjectParentConstructor(values, object, interpreter);


}
