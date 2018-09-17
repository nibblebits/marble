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

#include "commonmod_vector.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_Vector::CommonModule_Vector(Class *c) : Object(c)
{
}

CommonModule_Vector::~CommonModule_Vector()
{
}

std::shared_ptr<Object> CommonModule_Vector::newInstance(Class *c)
{
    return std::make_shared<CommonModule_Vector>(c);
}

Class *CommonModule_Vector::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Vector extends List
     * 
     * The Vector class is a class that provided List functionality. This allows one to push and pop the Value objects which hold values of any type.
     */
    
    Class *c = moduleSystem->getClassSystem()->registerClass("Vector", moduleSystem->getClassSystem()->getClassByName("List"));
    c->setDescriptorObject(std::make_shared<CommonModule_Vector>(c));

    // Register the constructor
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Vector
     * 
     * function push(Value v) : void
     * 
     * Pushes the Value to the Vector
     */
    Function *push_function = c->registerFunction("push", {VarType::fromString("Value")}, VarType::fromString("void"), CommonModule_Vector::Vector_Push);

    /**
     * @class Vector
     * 
     * function pop() : Value
     * 
     * Pops the Value from this Vector and returns it
     */
    Function *pop_function = c->registerFunction("pop", {}, VarType::fromString("Value"), CommonModule_Vector::Vector_Pop);

    /**
     * @class Vector
     * function back() : Value
     * 
     * Returns the value at the back of this Vector
     */
    Function *back_function = c->registerFunction("back", {}, VarType::fromString("Value"), CommonModule_Vector::Vector_Back);


    /**
     * @class Vector
     * function get(number index) : Value
     * 
     * Gets the Value with the provided index from the vector
     */
    Function *get_function = c->registerFunction("get", {VarType::fromString("number")}, VarType::fromString("Value"), CommonModule_Vector::Vector_Get);


    /**
     * @class Vector
     * function isEmpty() : boolean
     * 
     * Returns true if this Vector is empty otherwise false
     */
    Function *isEmpty_function = c->registerFunction("isEmpty", {}, VarType::fromString("boolean"), CommonModule_Vector::Vector_isEmpty);

    /**
     * @class Vector
     * function size() : number
     * 
     * Returns the size of this Vector
     */
    Function *size_function = c->registerFunction("size", {}, VarType::fromString("number"), CommonModule_Vector::Vector_Size);



}

void CommonModule_Vector::Vector_Push(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
    values[0].holder = NULL;
    vector_obj->vec_values.push_back(Value(&values[0]));
}

void CommonModule_Vector::Vector_Pop(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
    if (vector_obj->vec_values.empty())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "This Vector is empty", interpreter->getStackTraceLog());

    return_value->set(&vector_obj->vec_values.back());
    vector_obj->vec_values.pop_back();
}

void CommonModule_Vector::Vector_Back(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
    if (vector_obj->vec_values.empty())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "This Vector is empty", interpreter->getStackTraceLog());

    return_value->set(&vector_obj->vec_values.back());
}

void CommonModule_Vector::Vector_Get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
   if (vector_obj->vec_values.size() < values[0].dvalue)
   {
       throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "The index: " + std::to_string(values[0].dvalue) + " could not be found in Vector", interpreter->getStackTraceLog());
   }
   return_value->set(&vector_obj->vec_values[values[0].dvalue]);
}

void CommonModule_Vector::Vector_isEmpty(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
    return_value->set(vector_obj->vec_values.empty());
}

void CommonModule_Vector::Vector_Size(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Vector> vector_obj = std::dynamic_pointer_cast<CommonModule_Vector>(object);
    return_value->set(vector_obj->vec_values.size());
}