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

#include "trynode.h"
#include "interpreter.h"
#include "object.h"
#include "class.h"
#include "exceptionobject.h"
#include "nodes.h"
#include "exceptions/systemexception.h"
#include "exceptions/testerror.h"

TryNode::TryNode() : InterpretableNode(NODE_TYPE_TRY)
{
    this->try_body = NULL;
    this->catch_varnode = NULL;
    this->catch_body = NULL;
    this->finally_body = NULL;
}

TryNode::~TryNode()
{

}

void TryNode::test(Validator* validator, struct extras extra)
{
    this->try_body->test(validator);
    this->catch_varnode->test(validator);
    if(!this->catch_varnode->isObject())
        throw TestError("Primitive types are not allowed for exceptions");
    if (this->catch_varnode->isArray())
        throw TestError("The exception catch variable must not be an array");
    if (this->catch_varnode->value != NULL)
        throw TestError("Exception variables must not hold a default value");
   
    ClassSystem* c_system = validator->getClassSystem();
    struct Evaluation evaluation = this->catch_varnode->evaluate(validator, EVALUATION_TYPE_DATATYPE);
    Class* c = c_system->getClassByName(evaluation.datatype.value);
    Class* exception_c = c_system->getClassByName("Exception");
    if (!c->instanceOf(exception_c))
    {
        throw TestError("The class " + c->name + " is not an instance of the class Exception. Exceptions must extend the Exception class");
    }
         
    this->catch_body->test(validator);

    if (this->finally_body != NULL)
        this->finally_body->test(validator);
}

Value TryNode::interpret(Interpreter* interpreter, struct extras extra)
{
    try
    {
        this->try_body->interpret(interpreter);
    }
    catch(SystemException& ex)
    {
        std::shared_ptr<ExceptionObject> thrown_object = ex.getObject();
        // Let's see if we can actually catch this, if not we will rethrow the exception
        Class* thrown_exception_class = thrown_object->getClass();
        struct Evaluation evaluation = this->catch_varnode->evaluate(interpreter, EVALUATION_TYPE_DATATYPE);
        Class* exception_class = interpreter->getClassSystem()->getClassByName(evaluation.datatype.value);
        if (!thrown_exception_class->instanceOf(exception_class))
        {
            if (this->finally_body != NULL)
                this->finally_body->interpret(interpreter);
            // Ok the exception that was thrown is not an instance of our defined exception class so lets just rethrow it so it can be dealt with else where
            throw SystemException(thrown_object);
        }
        
        // Let's create an exception value that will point to the exception object        
        Value exception_value;
        exception_value.type = VALUE_TYPE_OBJECT;
        exception_value.ovalue = thrown_object;
        
        
        // Let's interpret the exception variable node and then set the value to the exception's object value that was thrown.
        Value v = this->catch_varnode->interpret(interpreter);
        v.holder->value.set(&exception_value);
        
        // We now must interpret the catch body
        this->catch_body->interpret(interpreter);
    }

    if (this->finally_body != NULL)
        this->finally_body->interpret(interpreter);
    Value v;
    return v;
}

void TryNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of try nodes is not supported");
}

