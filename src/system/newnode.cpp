#include "newnode.h"
#include "nodes.h"
#include "variable.h"
#include "array.h"
#include "interpreter.h"
#include <iostream>
NewNode::NewNode() : ExpressionInterpretableNode(NODE_TYPE_NEW)
{
    this->exp = NULL;
}

NewNode::~NewNode()
{

}

Value NewNode::interpret(Interpreter* interpreter)
{
    Value v;
    if (this->exp->type == NODE_TYPE_ARRAY)
    {
        v.type = VALUE_TYPE_ARRAY;
        
        /* NOTE HERE ONLY ONE DIMENSIONAL ARRAYS ARE SUPPORTED, IMPROVE THIS*/
        ArrayNode* array_node = (ArrayNode*) this->exp;
        // Lets evaluate the index of the array node
        Value index_value = array_node->index_node->interpret(interpreter);
        int var_type = VARIABLE_TYPE_OBJECT;
        Node* type_node = array_node->next_element;
        if (type_node->type == NODE_TYPE_KEYWORD)
        {
            KeywordNode* keyword_type = (KeywordNode*) type_node;
            var_type = interpreter->getVariableTypeForString(keyword_type->value);
        }
        // The index_value holds how many elements we need to create
        int total_elements = index_value.dvalue;
        
        Variable* variables = new Variable[total_elements];
        for (int i = 0; i < total_elements; i++)
        {
            Variable* var = &variables[i];
            var->type = var_type;
            var->value.holder = var;
        }
        v.avalue = new Array(variables, total_elements);
        // Arrays are also objects so lets make the ovalue an alias for when you dont care if its an object or an array as long as its one of them.
        v.ovalue = v.avalue;
        interpreter->getObjectManager()->incrementUseage(v.avalue);
    }
    return v;
}
