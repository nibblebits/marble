#ifndef STATEMENT_H
#define STATEMENT_H
#include "node.h"
#include "inode.h"
#include "bodynode.h"
class Statement : public InterpretableNode
{
public:
    Statement(int type);
    virtual ~Statement();
    BodyNode* body;
};
#endif
