#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "statics.h"
#include "einode.h"
class ExpNode : public ExpressionInterpretableNode
{
public:
    ExpNode();
    virtual ~ExpNode();

    ExpressionInterpretableNode* left;
    ExpressionInterpretableNode* right;
    std::string op;
    bool isAssignmentOperator();

    virtual Value interpret();
private:
};
#endif
