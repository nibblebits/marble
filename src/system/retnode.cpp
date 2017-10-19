#include "retnode.h"
#include "einode.h"
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
    return v;
}

void ReturnNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluating of return nodes is not yet implemented");
}
