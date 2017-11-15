#ifndef THROWNODE_H
#define THROWNODE_H
#include "inode.h"
#include <memory>
class ExpressionInterpretableNode;
class ThrowNode : public InterpretableNode
{
    public:
        ThrowNode();
        virtual ~ThrowNode();
        virtual void test(Validator* validator);
        virtual Value interpret(Interpreter* interpreter);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        
        /** The NewNode expression that should be interpreted and then have its object value thrown */
        ExpressionInterpretableNode* exp;
};
#endif
