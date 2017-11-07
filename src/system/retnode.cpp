#include "retnode.h"
#include "einode.h"
#include "functionsystem.h"
#include "writtenfunction.h"
#include "interpreter.h"

#include <iostream>

ReturnNode::ReturnNode() : InterpretableNode(NODE_TYPE_RETURN)
{
    this->exp = NULL;
}

ReturnNode::~ReturnNode()
{

}

Value ReturnNode::interpret(Interpreter* interpreter)
{
    Value v;
    if (this->exp != NULL)
        v = this->exp->interpret(interpreter);
    FunctionSystem* function_system = interpreter->getFunctionSystem();
    if (function_system->isInFunction())
    {
        WrittenFunction* current_function = (WrittenFunction*) function_system->getCurrentFunction();
        current_function->return_node = this;
        current_function->return_value = v;
    }
    return v;
}

void ReturnNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluating of return nodes is not yet implemented");
}
