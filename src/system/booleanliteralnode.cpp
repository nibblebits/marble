#include "booleanliteralnode.h"
#include "variable.h"
#include "exceptions/testerror.h"
#include "validator.h"
BooleanLiteralNode::BooleanLiteralNode() : ExpressionInterpretableNode(NODE_TYPE_BOOLEAN_LITERAL)
{
    this->literal = false;
}

BooleanLiteralNode::~BooleanLiteralNode()
{

}

void BooleanLiteralNode::test(Validator* validator, struct extras extra)
{
    if (validator->isExpecting() && validator->getExpectingType() != "boolean")
    {
        throw TestError("but a boolean literal was provided");
    }
}

Value BooleanLiteralNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.set(this->literal);
    return v;
}

void BooleanLiteralNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // The keyword node only evaluates data types.
    if (!(expected_evaluation & EVALUATION_TYPE_DATATYPE))
        return;
    
        
    evaluation->type |= EVALUATION_TYPE_DATATYPE;
    evaluation->datatype.type = Variable::getVariableTypeForString("boolean");
    evaluation->datatype.value = (this->literal ? "true" : "false");
}