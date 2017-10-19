#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "node.h"
#include "einode.h"
#include "value.h"
#include <vector>
class IdentifierNode;
class Interpreter;
class FunctionCallNode : public ExpressionInterpretableNode
{
public:
    FunctionCallNode();
    virtual ~FunctionCallNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    IdentifierNode* name;
    std::vector<ExpressionInterpretableNode*> arguments;
};
#endif
