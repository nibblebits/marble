#include "evaluatingnode.h"
EvaluatingNode::EvaluatingNode(NODE_TYPE type) : Node(type)
{

}

EvaluatingNode::~EvaluatingNode()
{

}

struct Evaluation EvaluatingNode::evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation)
{
    struct Evaluation evaluation;
    evaluate(handler, expected_evaluation, &evaluation);
    return evaluation;
}

void EvaluatingNode::evaluate(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    evaluate_impl(handler, expected_evaluation, evaluation);
}
