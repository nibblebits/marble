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

#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "nodes.h"
#include "singlefunction.h"
#include "vartype.h"
#include "debug.h"
#include "exceptionobject.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "exceptions/evaluationexception.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include <iostream>
#include <stdexcept>
FunctionCallNode::FunctionCallNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION_CALL)
{
    this->name = NULL;
}

FunctionCallNode::~FunctionCallNode()
{
}

void FunctionCallNode::test_args(Validator *validator, std::vector<VarType> *types, struct extras extra)
{
    struct extras arg_extra = extra;
    arg_extra.accessors_scope = extra.accessors_scope;

    // We must use the global function system for function arguments
    validator->useFunctionSystem(validator->getGlobalFunctionSystem(), [&] {
        validator->useScope([&] {
            for (ExpressionInterpretableNode *argument_node : this->arguments)
            {
                argument_node->test(validator, arg_extra);
                try
                {
                    struct Evaluation evaluation;
                    evaluation.extra = arg_extra;
                    argument_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE, &evaluation);
                    types->push_back(evaluation.datatype);
                }
                catch (EvaluationException &ex)
                {
                    throw TestError("The value provided for the function call is multityped. Ensure you cast where types differ. First type is: " + ex.type1 + " second is: " + ex.type2);
                }
            }
        },
                            extra.accessors_scope);
    });
}

void FunctionCallNode::test(Validator *validator, struct extras extra)
{
    if (shouldIgnoreValidation())
    {
        return;
    }

    /*
    * If the accessors scope is NULL then we will default to the current validators scope
    */

    if (extra.accessors_scope == NULL)
    {
        extra.accessors_scope = validator->getCurrentScope();
    }

    // Lets ensure the function actually exists
    FunctionSystem *function_sys = validator->getFunctionSystem();
    std::vector<VarType> types;
    validator->save();
    test_args(validator, &types, extra);
    validator->restore();
    /**
    * If we are not accessing an object we must default to the global function system
    */
    if (!extra.is_object_exp)
        function_sys = validator->getGlobalFunctionSystem();

    if (!function_sys->hasFunction(this->name->value, types, NULL, true, validator))
    {
        if (!function_sys->hasFunction(this->name->value))
        {
            throw TestError("The function \"" + this->name->value + "\" has not been declared");
        }
        throw TestError("The function \"" + this->name->value + "\" has not been declared that takes the given arguments");
    }

    SingleFunction *function = (SingleFunction *)function_sys->getFunctionByNameAndArguments(this->name->value, types, NULL, true, validator);
    if (validator->isExpecting())
    {
        // We must check the return type is valid
        VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();
        if (function->return_type.type != expecting_type)
            throw TestError("The function " + this->name->value + " returns a " + function->return_type.value);

        if (expecting_type == VARIABLE_TYPE_OBJECT)
        {
            if (!validator->getClassSystem()->isClassInstanceOf(function->return_type.value, validator->getExpectingType()))
                throw TestError("The function " + function->name + " returns a " + function->return_type.value);
        }

        if (function->return_type.isArray())
        {
            if (!validator->isExpectingArray())
                throw TestError("The function " + function->name + " returns an array of " + std::to_string(function->return_type.dimensions) + " dimensions of type " + function->return_type.value);
        }
    }

    int expected_dimensions = validator->getExpectedArrayDimensions();
    if (validator->isExpectingArray())
    {
        if (!function->return_type.isArray())
        {
            throw TestError("The function " + function->name + " returns a " + function->return_type.value + " but the caller is expecting an array of this type with " + std::to_string(expected_dimensions) + " dimensions");
        }
        else
        {
            if (function->return_type.dimensions != expected_dimensions)
            {
                throw TestError("The caller of function " + function->name + " is expecting " + std::to_string(expected_dimensions) + " dimensions but the function returns an array of " + function->return_type.value + " with " + std::to_string(function->return_type.dimensions) + " dimensions");
            }
        }
    }
    /**
    * We must now ensure that we can access this function, e.g it is public or if it is protected or private we must check we are allowed to access it
    */
    if (function->access != MODIFIER_ACCESS_PUBLIC)
    {
        if (!validator->isInClass())
        {
            throw TestError("The function " + function->name + " is private or protected in class: " + function->cls->name);
        }

        Class *our_class = validator->getCurrentClass();
        if (function->access == MODIFIER_ACCESS_PRIVATE)
        {
            /*
            * If the function we are calling has private access then we have to ensure that the class that we are in equals the class of the function
            */
            if (our_class != function->cls)
                throw TestError("The function: " + function->name + " is private in the class " + function->cls->name);
        }
        else
        {
            /**
             * This function is protected access so we must make sure we are an instance of the function class
             */
            if (!our_class->instanceOf(function->cls))
                throw TestError("The function: " + function->name + " is protected in the class " + function->cls->name);
        }
    }
}

Value FunctionCallNode::interpret(Interpreter *interpreter, struct extras extra)
{
    // Let's see if we have timed out
    interpreter->checkTimeout();

    Value value;
    std::vector<Value> argument_results;

    /*
    * If the accessors scope is NULL then we will default to the current interpreters scope.
    */
    if (extra.accessors_scope == NULL)
    {
        if (extra.is_object_exp)
            throw std::logic_error("Problem accessors scope is NULL but this is an object expression");
        extra.accessors_scope = interpreter->getCurrentScope();
        
    }

    /* If accessing an object the current scope
    * may have become something else so we need to use the accessors scope which is where the function call took place */
    interpreter->useScope([&] {
        interpret_args(interpreter, &argument_results, extra);
    },
                          extra.accessors_scope);

    
    interpreter->setLastFunctionCallNode(this);
    FunctionSystem *functionSystem = interpreter->getFunctionSystem();
    Function *function = NULL;
    if (extra.is_object_exp)
        function = functionSystem->getFunctionByName(name->value);
    else
        function = interpreter->getGlobalFunctionSystem()->getFunctionByName(name->value);

    if (function == NULL)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("EntityNotRegisteredException"), {})), "The function with the name " + name->value + " has not been registered", interpreter->getStackTraceLog());
    }

    try
    {
        function->invoke(interpreter, argument_results, &value, extra.current_object, extra.accessors_scope);
    }
    catch (SystemException &ex)
    {
        throw ex;
    }


    return value;
}

void FunctionCallNode::interpret_args(Interpreter *interpreter, std::vector<Value> *argument_results, struct extras extra)
{
    struct extras arg_extra = extra;
    arg_extra.accessors_scope = extra.accessors_scope;
    // Function arguments must use the global function system
    interpreter->useFunctionSystem(interpreter->getGlobalFunctionSystem(), [&] {
        for (ExpressionInterpretableNode *argument_node : this->arguments)
        {
            Value v = argument_node->interpret(interpreter, arg_extra);
            argument_results->push_back(v);
        }
    });
}

void FunctionCallNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{

    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        FunctionSystem *function_sys = handler->getFunctionSystem();
        std::vector<VarType> types;
        handler->useScope([&] {
            for (ExpressionInterpretableNode *argument_node : this->arguments)
            {
                argument_node->evaluate(handler, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE, evaluation);
                types.push_back(evaluation->datatype);
            }
        },
                          evaluation->extra.accessors_scope);

        SingleFunction *function;
        if (evaluation->extra.is_object_exp)
        {
            Class* a = (Class*) function_sys;
            function = (SingleFunction *)function_sys->getFunctionByNameAndArguments(this->name->value, types);
        }
        else
        {
            function = (SingleFunction *)handler->getGlobalFunctionSystem()->getFunctionByNameAndArguments(this->name->value, types);
        }

        if (function == NULL)
        {
            if (this->shouldIgnoreValidation())
            {
                // We can't get the datatype of a function call we do not know exists so let's just return
                return;
            }
            else
            {
                throw std::logic_error("The function call to evaluate has a call to a function that does not exist: " + this->name->value + ", is object exp: " + std::to_string(evaluation->extra.is_object_exp));
            }
        }
        evaluation->datatype = function->return_type;
    }
}
