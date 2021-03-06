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

#include "newnode.h"
#include "nodes.h"
#include "variable.h"
#include "array.h"
#include "interpreter.h"
#include "fcnode.h"
#include "validator.h"
#include "function.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>
#include <memory>
NewNode::NewNode() : ExpressionInterpretableNode(NODE_TYPE_NEW)
{
    this->type_node = NULL;
}

NewNode::~NewNode()
{

}

void NewNode::test(Validator* validator, struct extras extra)
{
    if (!validator->isExpecting())
        return;
    if (validator->isExpectingArray())
        test_for_array(validator);
    else
        test_for_object(validator);
}

void NewNode::test_for_object(Validator* validator)
{
    VALUE_TYPE expecting_type = validator->getExpectingValueType();
    std::string expecting_object = validator->getExpectingType();
    // We handle non-array objects only here. So if our type is an array then there is a test error
    if (isArray())
        throw TestError("an array was provided");

    if (expecting_type != VALUE_TYPE_OBJECT)
    {
        throw TestError("an object was provided");
    }
    
    if (expecting_object != "")
    {
        /* It is natural for this type node to be a function call node as it will be calling a constructor, e.g new Object();
     * the difference is we will not be interpreting instead we will be using it*/ 
        FunctionCallNode* fc_node = (FunctionCallNode*) this->type_node;
        if (!fc_node->shouldIgnoreValidation())
        {
            ClassSystem* class_sys = validator->getClassSystem();
            Class* expecting_class = class_sys->getClassByName(expecting_object);
            Class* fc_node_class = class_sys->getClassByName(fc_node->name->value);

            if (fc_node_class == NULL)
                throw TestError("The class with the name \"" + fc_node->name->value + "\" has not been declared");

            if (fc_node_class->is_pure)
            {
                throw TestError("The class " + fc_node_class->name + " could not be initialised because it is a pure class and contains pure methods. Extend this class and implement the pure methods then initialise that class instead");
            }
            
            if (expecting_object != fc_node->name->value && !fc_node_class->instanceOf(expecting_class))
            {
                throw TestError("a " + fc_node->name->value + " was provided which does not extend " + expecting_object);
            }

            // Now lets test the constructor to see if a valid constructor for us to be able to call.
            std::vector<VarType> types;
            validator->save();
            fc_node->test_args(validator, &types);

            // If we don't have any constructor then we can ignore this validation as no constructors is legal
            if (fc_node_class->hasFunction("__construct"))
            {
                if (!fc_node_class->hasFunctionLocally("__construct", types, true, validator))
                {
                    throw TestError("A constructor with the given arguments does not exist within the " + fc_node_class->name + " class");
                }
            }
            validator->restore();

        }
    }
}

void NewNode::test_for_array(Validator* validator)
{
    int expected_dimensions = validator->getExpectedArrayDimensions();
    int current_dimensions = this->array_values.size();
    if (expected_dimensions != current_dimensions)
        throw TestError("the array dimensions do not match. Expecting " + std::to_string(expected_dimensions) + " dimensions but " + std::to_string(current_dimensions) + " provided");
        
    std::string expecting_type = validator->getExpectingType();
    struct Evaluation eval = this->type_node->evaluate(validator, EVALUATION_TYPE_DATATYPE);
    if(expecting_type != eval.datatype.value)
    {
        VALUE_TYPE expecting_value_type = validator->getExpectingValueType();
        if (expecting_value_type == VALUE_TYPE_OBJECT 
            && eval.datatype.type == VALUE_TYPE_OBJECT)
        {
            // Both the expecting value and the value provided are possibly objects so this may still be a legal array if it is an instance of the array class.
            ClassSystem* class_sys = validator->getClassSystem();
            Class* expecting_class = class_sys->getClassByName(expecting_type);
            Class* value_class = class_sys->getClassByName(eval.datatype.value);
            
            // Is it an instance? If so its completely legal so return
            if (value_class->instanceOf(expecting_class))
                return;
        }
        throw TestError("an array type of " + eval.datatype.value + " was provided");
    }   
}

bool NewNode::isArray()
{
    return !this->array_values.empty();
}

std::shared_ptr<Array> NewNode::new_variable_array(Interpreter* interpreter, int var_type, int total_elements)
{
    Variable* variables = new Variable[total_elements];
    for (int i = 0; i < total_elements; i++)
    {
        Variable* var = &variables[i];
        var->type = var_type;
        var->value.type = Value::getValueTypeFromVariableType(var_type);
        var->value.holder = var;
    }
    return std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, total_elements);
}


void NewNode::new_object_variable(Interpreter* interpreter, Value& v, FunctionCallNode* fc_node)
{
    Class* object_class = interpreter->getClassSystem()->getClassByName(fc_node->name->value);
    std::vector<Value> argument_results;
    for (ExpressionInterpretableNode* argument_node : fc_node->arguments)
    {
        Value v = argument_node->interpret(interpreter);
        argument_results.push_back(v);
    }

    std::shared_ptr<Object> object = NULL;
    try
    {
       object = Object::create(interpreter, object_class, argument_results);
    }
    catch(SystemException& ex)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Exception"), {})), ex.getObject()->getMessage() + " for class: " + fc_node->name->value, interpreter->getStackTraceLog());
    }
    v.set(object);
}

void NewNode::handle_new_variable(Interpreter* interpreter, Value& v)
{
    /* It is natural for this type node to be a function call node as it will be calling a constructor, e.g new Object();
     * the difference is we will not be interpreting instead we will be using it*/ 
    FunctionCallNode* fc_node = (FunctionCallNode*) this->type_node;
    new_object_variable(interpreter, v, fc_node);
}

std::shared_ptr<Array> NewNode::new_array_array(Interpreter* interpreter, int total_elements, std::vector<ExpressionInterpretableNode*>::iterator it)
{
    Variable* variables = new Variable[total_elements];
    for (int i = 0; i < total_elements; i++)
    {
        Variable* var = &variables[i];
        var->type = VARIABLE_TYPE_OBJECT;
        var->value.type = VALUE_TYPE_ARRAY;
        var->value.type_str = "array";
        // Each element of this array will contain a reference to the next array
        handle_array(interpreter, var->value, it+1);
        var->value.holder = var;
    }
    std::shared_ptr<Array> array = std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, total_elements);
    return array;
}


void NewNode::handle_array(Interpreter* interpreter, Value& v, std::vector<ExpressionInterpretableNode*>::iterator it)
{
    v.type = VALUE_TYPE_ARRAY;
    int total_elements = (*it)->interpret(interpreter).dvalue;
    if (it == array_values.end()-1)
    {
        int var_type = -1;
        // This is the final element it must be where the actual variables are stored
        switch(type_node->type)
        {
            case NODE_TYPE_KEYWORD:
            {
                KeywordNode* keyword_type = (KeywordNode*) type_node;
                var_type = Variable::getVariableTypeForString(keyword_type->value);
                v.avalue = new_variable_array(interpreter, var_type, total_elements);            
            }
            break;
            
            case NODE_TYPE_IDENTIFIER:
            {
                IdentifierNode* identifier_node = (IdentifierNode*) type_node;
                var_type = VARIABLE_TYPE_OBJECT;
                v.avalue = new_variable_array(interpreter, var_type, total_elements);
            }
            break;
        }
    }
    else
    {
        // This is another array of arrays
        v.avalue = new_array_array(interpreter, total_elements, it);
    }
    // Arrays are also objects so lets make the ovalue an alias for when you dont care if its an object or an array as long as its one of them.
    v.ovalue = v.avalue;
 
}
Value NewNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    if (this->isArray())
    {
        handle_array(interpreter, v, this->array_values.begin());
    }
    else
    {
       handle_new_variable(interpreter, v);
    }
    return v;
}


void NewNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        if(this->type_node->type == NODE_TYPE_FUNCTION_CALL)
        {
            FunctionCallNode* fc_node = (FunctionCallNode*) this->type_node;
            evaluation->datatype = VarType::fromString(fc_node->name->value);
        }
        else
        {
            this->type_node->evaluate(handler, expected_evaluation, evaluation);
        }
    }
}

