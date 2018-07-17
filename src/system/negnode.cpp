#include "negnode.h"
#include "nodes.h"
#include <iostream>
NegNode::NegNode() : ExpressionInterpretableNode(NODE_TYPE_NEGATIVE)
{
    this->node = NULL;
}

NegNode::~NegNode()
{

}

Value NegNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = -this->node->interpret(interpreter).dvalue;
    return v;

}

void NegNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        // Since this is a negative literal node and they are expecting a datatype we should return a "number" type as this is the standard type for literal nodes.
        evaluation->datatype.type = VARIABLE_TYPE_NUMBER;
        evaluation->datatype.value = "number"; 
    }
}
