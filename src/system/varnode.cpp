#include "varnode.h"
#include "nodes.h"
#include "variable.h"
#include "debug.h"
#include "interpreter.h"
#include "value.h"
#include "validator.h"
#include "function.h"
#include "exceptions/testerror.h"
#include <iostream>
VarNode::VarNode() : InterpretableNode(NODE_TYPE_VARIABLE_DECLARATION)
{
    this->type = NULL;
    this->name = "";
    this->value = NULL;
    this->dimensions = 0;
    this->access = MODIFIER_ACCESS_PUBLIC;
}

VarNode::~VarNode()
{
}

/*
* Validates that this variable node is valid and safe to use.
*/

void VarNode::test(Validator *validator, struct extras extra)
{
    std::string type_str = getTypeAsString();
    if (validator->isExpecting())
    {
        if (type_str != validator->getExpectingType())
            throw TestError("This variable must be of type " + validator->getExpectingType() + " but a " + type_str + " was provided");
    }

    // We need to check if the variable already exists in this scope
    if (validator->getCurrentScope()->getVariable(this->name) != NULL)
    {
        throw TestError("a variable with the name: \"" + this->name + "\" has already been declared in this scope");
    }

    if (!isPrimitive())
    {
        // Let's ensure this object exists
        ClassSystem *class_sys = validator->getClassSystem();
        if (!shouldIgnoreValidation())
        {
            if (!class_sys->hasClassWithName(type_str))
                throw TestError("The class with the name \"" + type_str + "\" has not been declared");
        }
        else
            validator->ignoreClass(type_str);
    }

    if (this->value != NULL)
    {
        validator->save();
        bool ignore_expecting = false;
        if (Variable::getVariableTypeForString(type_str) == VARIABLE_TYPE_OBJECT)
        {
            /* Let's just check if this class has overridden the "=" operator 
            * if it does we should not instruct the validator to expects a given type
            */
            Class *c = validator->getClassSystem()->getClassByName(type_str);

            struct Evaluation evaluation = this->value->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
            if (c->hasOverloadedOperator("=", evaluation.datatype.value))
            {
                ignore_expecting = true;
            }  
        }

        if (!ignore_expecting)
        {
            validator->expecting(type_str);
            if (isArray())
                validator->expectingArray(this->dimensions);
        }

        try
        {
            this->value->test(validator);
        }
        catch (TestError &ex)
        {
            throw TestError("Expecting a " + type_str + (isArray() ? " array" : "") + "; " + ex.what());
        }

        if (!ignore_expecting)
            validator->endExpecting();
        validator->restore();
    }

    Variable *var = validator->getCurrentScope()->createVariable();
    var->type = Variable::getVariableTypeForString(type_str);
    var->dimensions = this->dimensions;
    var->access = this->access;
    var->type_name = type_str;
    var->name = this->name;
}

void VarNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        evaluation->datatype.type = type->type;
        evaluation->datatype.value = getTypeAsString();
    }
}

std::string VarNode::getTypeAsString()
{
    std::string type_str;
    switch (type->type)
    {
    case NODE_TYPE_KEYWORD:
    {
        type_str = ((KeywordNode *)type)->value;
    }
    break;

    case NODE_TYPE_IDENTIFIER:
    {
        type_str = ((IdentifierNode *)type)->value;
    }
    break;

    default:
        throw std::logic_error("Unsure how to get type as string");
    }

    return type_str;
}

bool VarNode::isArray()
{
    return this->dimensions > 0;
}

bool VarNode::isPrimitive()
{
    return this->type->type == NODE_TYPE_KEYWORD;
}

bool VarNode::isObject()
{
    return !isPrimitive();
}


bool VarNode::handleOperatorOverloadIfValid(Interpreter* interpreter, std::string type_str, Variable* variable, ExpressionInterpretableNode* value_node)
{
    if (Variable::getVariableTypeForString(type_str) == VARIABLE_TYPE_OBJECT)
    {
        Class *c = interpreter->getClassSystem()->getClassByName(type_str);

        struct Evaluation evaluation = this->value->evaluate(interpreter, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        if (c->hasOverloadedOperator("=", evaluation.datatype.value))
        {
           /* Ok we have an equal operator that has been overloaded so let's call the overloaded operator function
            * we will use the value returned to set to this variables value
            */
           Function* f = c->getFunctionByNameAndArguments("operator:=", {evaluation.datatype});
           Value value_node_value = value_node->interpret(interpreter);
           Value return_value;
           f->invoke(interpreter, {value_node_value}, &return_value, NULL, interpreter->getCurrentScope());
           variable->value = return_value;
           variable->value.holder = variable;
           return true;
        }
    }

    return false;
}

/*
* Interprets the variable node and creates a variable
* then it returns the value of the variable that it just set.
*/
Value VarNode::interpret(Interpreter *interpreter, struct extras extra)
{
    Node *type_node = type;
    std::string type_str = getTypeAsString();

    ExpressionInterpretableNode *value_node = (ExpressionInterpretableNode *)value;
    Variable *variable = interpreter->getCurrentScope()->createVariable();
    // Is this an operator overload assignment? If not process it as a normal assignment
    if (!handleOperatorOverloadIfValid(interpreter, type_str, variable, value_node))
    {
        if (value_node != NULL)
        {
            variable->value = value_node->interpret(interpreter);
            variable->value.holder = variable;
        }
        else
        {
            variable->value.type = Value::getValueTypeForString(type_str);
        }
    }

    variable->value.holder = variable;
    variable->name = name;
    variable->access = this->access;
    variable->dimensions = this->dimensions;
    variable->type = Variable::getVariableTypeForString(type_str);
    variable->type_name = type_str;
    variable->value.type_str = type_str;
    Value v = variable->value;
    return v;
}
