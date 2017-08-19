#ifndef STRINGNODE_H
#define STRINGNODE_H
#include "einode.h"
class StringNode : public ExpressionInterpretableNode
{
    public:
        StringNode();
        virtual ~StringNode();
        virtual Value interpret(Interpreter* interpreter);
        std::string value;
};
#endif
