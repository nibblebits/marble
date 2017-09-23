#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "node.h"
#include "einode.h"
#include "value.h"
#include <vector>
class IdentifierNode;
class Interpreter;
class FunctionCallNode : public ExpressionInterpretableNode
{
public:
    FunctionCallNode();
    virtual ~FunctionCallNode();
    virtual Value interpret(Interpreter* interpreter);
    IdentifierNode* name;
    std::vector<ExpressionInterpretableNode*> arguments;
};
#endif
