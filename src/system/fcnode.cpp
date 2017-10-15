#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "nodes.h"
#include "function.h"
#include "exceptions/testerror.h"
#include <iostream>
#include <stdexcept>
FunctionCallNode::FunctionCallNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION_CALL)
{
    this->name = NULL;
}

FunctionCallNode::~FunctionCallNode()
{

}


void FunctionCallNode::test(Validator* validator)
{
   // Lets ensure the function actually exists
   FunctionSystem* function_sys = validator->getFunctionSystem();
   if (!function_sys->hasFunction(this->name->value))
   {
       throw TestError("The function " + this->name->value + " has not been declared");
   }
   
   for (ExpressionInterpretableNode* argument_node : this->arguments)
   {
       argument_node->test(validator);
   }
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

   FunctionSystem* functionSystem = interpreter->getFunctionSystem();
   Function* function = functionSystem->getFunctionByName(name->value);
   if (function == NULL)
   {
        throw std::logic_error("Value FunctionCallNode::interpret(Interpreter* interpreter): Attempting to invoke a function that has not been registered");
   }

   // If the function invoked is not part 
   function->invoke(argument_results, &value, interpreter->getCurrentObject());
   return value;
}
