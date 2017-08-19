#ifndef IDENTIFIERNODE_H
#define IDENTIFIERNODE_H
#include "einode.h"
#include <string>
class IdentifierNode : public ExpressionInterpretableNode
{
public:
    IdentifierNode();
    virtual ~IdentifierNode();
    virtual Value interpret(Interpreter* interpreter);
    std::string value;
};

#endif