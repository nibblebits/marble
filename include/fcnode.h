#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "node.h"
#include "einode.h"
#include "value.h"
#include <vector>
class IdentifierNode;
class Interpreter;
class Function;
class Validator;
class Value;
class FunctionCallNode : public ExpressionInterpretableNode
{
public:
    FunctionCallNode();
    virtual ~FunctionCallNode();
    virtual void test(Validator* validator, struct extras extra);
    void test_args(Validator* validator, std::vector<VarType>* types, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    void interpret_args(Interpreter* interpreter, std::vector<Value>* argument_results, struct extras extra={});

    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    IdentifierNode* name;
    std::vector<ExpressionInterpretableNode*> arguments;
};
#endif
