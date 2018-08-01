#ifndef LIMITSCOPENODE_H
#define LIMITSCOPENODE_H
#include "statement.h"
#include "inode.h"
/**
 * This node limits the scope to the variable scope in limit_to. It can't go higher and access global variables
 */
class BodyNode;
class LimitScopeNode : public Statement
{
public:
    LimitScopeNode();
    virtual ~LimitScopeNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    BodyNode* limit_to;
    BodyNode* scope;
};
#endif
