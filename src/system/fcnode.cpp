#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
#include "nodes.h"
#include "function.h"
#include <iostream>
#include <stdexcept>
FunctionCallNode::FunctionCallNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION_CALL)
{
    this->dest = NULL;
}

FunctionCallNode::~FunctionCallNode()
{

}

Value FunctionCallNode::interpret(Interpreter* interpreter)
{
   Value value;

   std::vector<Value> argument_results;
   for (ExpressionInterpretableNode* argument_node : this->arguments)
   {
       Value v = argument_node->interpret(interpreter);
       argument_results.push_back(v);
   }

    if (this->dest->type == NODE_TYPE_IDENTIFIER)
    {
        IdentifierNode* iden_dest = (IdentifierNode*) this->dest;
        Function* function = interpreter->getFunctionSystem()->getFunctionByName(iden_dest->value);
        if (function == NULL)
        {
            throw std::logic_error("Value FunctionCallNode::interpret(Interpreter* interpreter): Attempting to invoke a function that has not been registered");
        }

        function->invoke(argument_results, &value);
    }
    
    return value;
}
