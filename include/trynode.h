#ifndef TRYNODE_H
#define TRYNODE_H
#include "inode.h"
#include <memory>
class Validator;
class Interpreter;
class BodyNode;
class VarNode;
class TryNode : public InterpretableNode
{
    public:
        TryNode();
        virtual ~TryNode();
        virtual void test(Validator* validator, struct extras extra);
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        BodyNode* try_body;
        BodyNode* finally_body;
        /** The variable node representing the variable that the exception that was thrown should be stored in */
        VarNode* catch_varnode;
        BodyNode* catch_body;
};
#endif
