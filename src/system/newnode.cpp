#include "newnode.h"
#include "nodes.h"
#include "variable.h"
#include "array.h"
#include "interpreter.h"
#include "fcnode.h"
#include <iostream>
#include <memory>
NewNode::NewNode() : ExpressionInterpretableNode(NODE_TYPE_NEW)
{
    this->type_node = NULL;
}

NewNode::~NewNode()
{

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
        var->value.holder = var;
    }
    return std::make_shared<Array>(interpreter, interpreter->getClassSystem()->getClassByName("array"), variables, total_elements);
}


void NewNode::new_object_variable(Interpreter* interpreter, Value& v, std::string class_name)
{
    std::shared_ptr<Object> object = std::make_shared<Object>(interpreter, interpreter->getClassSystem()->getClassByName(class_name));
    object->setup();
    v.type = VALUE_TYPE_OBJECT;
    v.ovalue = object;
}

void NewNode::handle_new_variable(Interpreter* interpreter, Value& v)
{
    /* It is natural for this type node to be a function call node as it will be calling a constructor, e.g new Object();
     * the difference is we will not be interpreting instead we will be using it*/ 
    FunctionCallNode* fc_node = (FunctionCallNode*) this->type_node;
    new_object_variable(interpreter, v, fc_node->name->value);
}

std::shared_ptr<Array> NewNode::new_array_array(Interpreter* interpreter, int total_elements, std::vector<ExpressionInterpretableNode*>::iterator it)
{
    Variable* variables = new Variable[total_elements];
    for (int i = 0; i < total_elements; i++)
    {
        Variable* var = &variables[i];
        var->type = VARIABLE_TYPE_ARRAY;
        // Each element of this array will contain a reference to the next array
        handle_array(interpreter, var->value, it+1);
        var->value.holder = var;
    }
    std::shared_ptr<Array> array = std::make_shared<Array>(interpreter, interpreter->getClassSystem()->getClassByName("array"), variables, total_elements);
    array->setup();
    return array;
}


void NewNode::handle_array(Interpreter* interpreter, Value& v, std::vector<ExpressionInterpretableNode*>::iterator it)
{
    v.type = NODE_TYPE_ARRAY;
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
Value NewNode::interpret(Interpreter* interpreter)
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
