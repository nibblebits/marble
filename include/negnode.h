#ifndef NEGNODE_H
#define NEGNODE_H

#include "einode.h"
class NegNode : public ExpressionInterpretableNode
{
    public:
        NegNode();
        virtual ~NegNode();
        ExpressionInterpretableNode* node;
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
