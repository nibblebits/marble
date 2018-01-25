#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H
#include "einode.h"
#include <string>
#include <vector>

class VarNode;
class BodyNode;
class FunctionNode : public ExpressionInterpretableNode
{
public:
    FunctionNode();
    virtual ~FunctionNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    std::string name;
    struct BodyNode* body;
    std::vector<VarNode*> args;
    bool is_pure;
    ExpressionInterpretableNode* return_type;
};
#endif
