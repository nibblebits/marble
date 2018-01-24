#include "includenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
IncludeNode::IncludeNode() : InterpretableNode(NODE_TYPE_INCLUDE)
{

}

IncludeNode::~IncludeNode()
{

}

void IncludeNode::test(Validator* validator)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting();
}

Value IncludeNode::interpret(Interpreter* interpreter)
{
    Value v = this->exp->interpret(interpreter);
    interpreter->runScript(v.svalue.c_str());
    return v;
}

void IncludeNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
