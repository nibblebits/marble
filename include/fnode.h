#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H
#include "einode.h"
#include <string>
#include <vector>

class VarNode;
class BodyNode;
class FunctionNode : public ExpressionInterpretableNode
{
public:
    FunctionNode();
    virtual ~FunctionNode();
    virtual Value interpret(Interpreter* interpreter);
    
    std::string name;
    struct BodyNode* body;
    std::vector<VarNode*> args;
    Node* return_type;
};
#endif