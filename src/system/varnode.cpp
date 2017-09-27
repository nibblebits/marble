#include "varnode.h"
#include "nodes.h"
#include "variable.h"
#include "debug.h"
#include "interpreter.h"
#include "value.h"
#include <iostream>
VarNode::VarNode() : InterpretableNode(NODE_TYPE_VARIABLE_DECLARATION)
{
    this->type = NULL;
    this->name = "";
    this->value = NULL;
    this->dimensions = 0;
}

VarNode::~VarNode()
{

}

/*
* Interprets the variable node and creates a variable
* then it returns the value of the variable that it just set.
*/
Value VarNode::interpret(Interpreter* interpreter)
{
    Node* type_node = type;
    std::string type_str = "";
    switch (type_node->type)
    {
        case NODE_TYPE_KEYWORD:
        {
            type_str = ((KeywordNode*)type)->value;
        }
        break;
        
        case NODE_TYPE_IDENTIFIER:
        {
            type_str = ((IdentifierNode*)type)->value;
        }
        break;
    }
    
    ExpressionInterpretableNode* value_node = (ExpressionInterpretableNode*)value;

    Variable* variable = interpreter->getCurrentScope()->createVariable();
    Debug::PrintValueForNode(value_node);
    if (value_node != NULL)
    {
        variable->value = value_node->interpret(interpreter);
    }
    else
    {
        variable->value.type = Value::getValueTypeForString(type_str);
    }
    variable->value.holder = variable;
    variable->name = name;
    variable->type = Variable::getVariableTypeForString(type_str);
    Value v = variable->value;
    return v;
}
