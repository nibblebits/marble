#ifndef STRINGNODE_H
#define STRINGNODE_H
#include "einode.h"
class Validator;
class StringNode : public ExpressionInterpretableNode
{
    public:
        StringNode();
        virtual ~StringNode();
        virtual void test(Validator* validator, struct extras extra);
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        std::string value;
};
#endif
