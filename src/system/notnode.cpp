#include "notnode.h"
#include "nodes.h"
#include "statics.h"
NotNode::NotNode() : ExpressionInterpretableNode(NODE_TYPE_NOT)
{
    this->node = NULL;
}

NotNode::~NotNode()
{

}

void NotNode::test(Validator* validator, struct extras extra)
{
    validator->save();
    validator->expecting("boolean");
    this->node->test(validator);
    validator->endExpecting();
    validator->restore();
}

Value NotNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = !this->node->interpret(interpreter).dvalue;
    return v;
}

void NotNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        evaluation->datatype.type = VARIABLE_TYPE_BOOLEAN;
        evaluation->datatype.value = "boolean"; 
    }}
