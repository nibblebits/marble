#ifndef IDENTIFIERNODE_H
#define IDENTIFIERNODE_H
#include "einode.h"
#include "statics.h"
#include <string>
class IdentifierNode : public ExpressionInterpretableNode
{
public:
    IdentifierNode();
    virtual ~IdentifierNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    std::string value;
};

#endif
