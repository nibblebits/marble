#include "evaluatingnode.h"
EvaluatingNode::EvaluatingNode(NODE_TYPE type) : Node(type)
{

}

EvaluatingNode::~EvaluatingNode()
{

}

struct Evaluation EvaluatingNode::evaluate(SystemHandler* handler)
{
    struct Evaluation evaluation;
    evaluate(handler, &evaluation);
    return evaluation;
}

void EvaluatingNode::evaluate(SystemHandler* handler, struct Evaluation* evaluation)
{
    evaluate_impl(handler, evaluation);
}
