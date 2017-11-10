#include "varnode.h"
#include "nodes.h"
#include "variable.h"
#include "debug.h"
#include "interpreter.h"
#include "value.h"
#include "validator.h"
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

void VarNode::test(Validator* validator)
{
   std::string type_str = getTypeAsString();
   if (this->value != NULL)
   {
       if (isArray())
       {
           validator->expectingArray(this->dimensions);
       }
       
       validator->expecting(type_str);
       if (!isPrimitive())
       {
           // Let's ensure this object exists
           ClassSystem* class_sys = validator->getClassSystem();
           if (!class_sys->hasClassWithName(type_str))
                throw TestError("The class with the name \"" + type_str + "\" has not been declared");
       }
       
       try
       {
         this->value->test(validator);
       } catch(TestError& ex)
       {
          throw TestError("Expecting a " + type_str + (isArray() ? " array" : "") + "; " + ex.what());
       }
   
       validator->endExpecting();
   }
   
   Variable* var = validator->getCurrentScope()->createVariable();
   var->type = Variable::getVariableTypeForString(type_str);
   var->dimensions = this->dimensions;
   var->access = this->access;
   var->type_name = type_str;
   var->name = this->name;
}

void VarNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation is not implemented for variable nodes");
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
    std::string type_str = getTypeAsString();
    return (type_str == "number" || type_str == "string");
}


bool VarNode::isObject()
{
    return !isPrimitive();
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
    variable->dimensions = this->dimensions;
    variable->type = Variable::getVariableTypeForString(type_str);
    variable->type_name = type_str;
    Value v = variable->value;
    return v;
}
