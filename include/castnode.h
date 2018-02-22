#ifndef CASTNODE_H
#define CASTNODE_H

#include "einode.h"
class Validator;
class EvaluatingNode;
class CastNode : public ExpressionInterpretableNode
{
public:
    CastNode();
    virtual ~CastNode();
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void test(Validator* validator, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    EvaluatingNode* casting_to;
    ExpressionInterpretableNode* to_cast;
private:
    std::string get_string_value(Value* from);
    double get_double_value(Value* from);
};
#endif
