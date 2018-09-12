#include "commonmod_list.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_List::CommonModule_List(Class* c) : Object(c)
{
    
}

CommonModule_List::~CommonModule_List()
{
    
}

std::shared_ptr<Object> CommonModule_List::newInstance(Class* c)
{
    return std::make_shared<CommonModule_List>(c);
}

Class* CommonModule_List::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class List
     * 
     * The List class is a pure class that holds is the base skeleton for all List classes that want to provide List functionality
     */ 
    Class* c = moduleSystem->getClassSystem()->registerClass("List");
    c->setDescriptorObject(std::make_shared<CommonModule_List>(c));
    c->is_pure = true;

    /**
     * @class List
     * function push(Value v) : void
     * 
     * Pushes the Value to the List
     */
    Function* push_function = c->registerFunction("push", {VarType::fromString("Value")}, VarType::fromString("void"), Function::Blank);
    push_function->is_pure = true;

    /**
     * @class List
     * function pop() : Value
     * 
     * Pops the Value from this list and returns it
     */
    Function* pop_function = c->registerFunction("pop", {}, VarType::fromString("Value"), Function::Blank);
    pop_function->is_pure = true;


    /**
     * @class List
     * function get(number index) : Value
     * 
     * Gets the Value with the provided index from the list
     */
    Function *get_function = c->registerFunction("get", {VarType::fromString("number")}, VarType::fromString("Value"), Function::Blank);
    get_function->is_pure = true;

    /**
     * @class List
     * function isEmpty() : boolean
     * 
     * Returns true if this List is empty otherwise false
     */
    Function *isEmpty_function = c->registerFunction("isEmpty", {}, VarType::fromString("boolean"), Function::Blank);
    isEmpty_function->is_pure = true;

    /**
     * @class List
     * function size() : number
     * 
     * Returns the size of this List
     */
    Function *size_function = c->registerFunction("size", {}, VarType::fromString("number"), Function::Blank);
    size_function->is_pure = true;


}