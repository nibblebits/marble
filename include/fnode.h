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
    virtual void test(Validator* validator, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    bool isArray();
    
    std::string name;
    struct BodyNode* body;
    std::vector<VarNode*> args;
    bool is_pure;
    ExpressionInterpretableNode* return_type;
    int dimensions;


    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
};
#endif
