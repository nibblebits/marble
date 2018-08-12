#ifndef NOTNODE_H
#define NOTNODE_H

#include "einode.h"
class NotNode : public ExpressionInterpretableNode
{
    public:
        NotNode();
        virtual ~NotNode();
        ExpressionInterpretableNode* node;
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
