#ifndef PERMISSIONNODE_H
#define PERMISSIONNODE_H
#include "einode.h"
#include <vector>

class ExpNode;
class BodyNode;
class PermissionNode : public ExpressionInterpretableNode
{
    public:
        PermissionNode();
        virtual ~PermissionNode();

        virtual Value interpret(Interpreter* interpreter, struct extras extra={});
        virtual void test(Validator* validator, struct extras extra={});
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        ExpNode* exp_node;
        BodyNode* body_node;
    private:

};
#endif