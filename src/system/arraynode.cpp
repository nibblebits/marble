#include "arraynode.h"
#include "variable.h"
#include "array.h"
ArrayNode::ArrayNode() : ExpressionInterpretableNode(NODE_TYPE_ARRAY)
{
    this->index_node = NULL;
    this->next_element = NULL;
}

ArrayNode::~ArrayNode()
{

}
    
Value ArrayNode::interpret(Interpreter* interpreter)
{
    Value index_exp = this->index_node->interpret(interpreter);
    // If this array node is being interpreted then the next_element is guaranteed to be an expression interpretable node.
    Value next_elem_value = ((ExpressionInterpretableNode*)this->next_element)->interpret(interpreter);
    if (index_exp.dvalue > 0xffffffff || index_exp.dvalue < 0)
    {
        throw std::logic_error("Array indexes must be below 0xffffffff and above zero");
    }
    else if(index_exp.dvalue >= next_elem_value.avalue->count)
    {
        throw std::logic_error("Index out of bounds");
    }
    return next_elem_value.avalue->variables[(int)index_exp.dvalue].value;
}
