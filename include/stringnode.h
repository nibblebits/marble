#ifndef STRINGNODE_H
#define STRINGNODE_H
#include "einode.h"
class Validator;
class StringNode : public ExpressionInterpretableNode
{
    public:
        StringNode();
        virtual ~StringNode();
        virtual void test(Validator* validator);
        virtual Value interpret(Interpreter* interpreter);
        std::string value;
};
#endif
