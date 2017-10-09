#include "varnode.h"
#include "nodes.h"
#include "variable.h"
#include "debug.h"
#include "interpreter.h"
#include "value.h"
#include "validator.h"
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

void VarNode::test(Validator* validator)
{
   std::string type_str = getTypeAsString();
   if (this->value != NULL)
   {
       if (type_str == "number")
          validator->expecting(VALUE_TYPE_NUMBER);
       else if(type_str == "string")
          validator->expecting(VALUE_TYPE_STRING);
       else
          validator->expectingObject(type_str);
          
       try
       {
         this->value->test(validator);
       } catch(std::logic_error& ex)
       {
          throw std::logic_error("Expecting a " + type_str + " but " + ex.what());
       }
   
       validator->endExpecting();
   }
   
   Variable* var = validator->getCurrentScope()->createVariable();
   var->type = Variable::getVariableTypeForString(type_str);
   var->type_name = type_str;
   var->name = this->name;
}

std::string VarNode::getTypeAsString()
{
    std::string type_str;
    switch (type->type)
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
    
    return type_str;
}
/*
* Interprets the variable node and creates a variable
* then it returns the value of the variable that it just set.
*/
Value VarNode::interpret(Interpreter* interpreter)
{
    Node* type_node = type;
    std::string type_str = getTypeAsString();
    
    ExpressionInterpretableNode* value_node = (ExpressionInterpretableNode*)value;

    Variable* variable = interpreter->getCurrentScope()->createVariable();
    Debug::PrintValueForNode(value_node);
    if (value_node != NULL)
    {
        variable->value = value_node->interpret(interpreter);
        variable->value.holder = variable;
    }
    else
    {
        variable->value.type = Value::getValueTypeForString(type_str);
    }
    variable->value.holder = variable;
    variable->name = name;
    variable->access = this->access;
    variable->type = Variable::getVariableTypeForString(type_str);
    variable->type_name = type_str;
    Value v = variable->value;
    return v;
}
