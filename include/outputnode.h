#ifndef OUTPUTNODE_H
#define OUTPUTNODE_H
#include "einode.h"

/**
 * This node signifies that the value from interpretting should be outputted to the output stream such as the terminal or web browser
 */
class OutputNode : public ExpressionInterpretableNode
{
public:
    OutputNode();
    virtual ~OutputNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    ExpressionInterpretableNode* exp;
};
#endif
