#ifndef STATEMENT_H
#define STATEMENT_H
#include "node.h"
#include "interpretable.h"
#include "bodynode.h"
class Statement : public Node
{
public:
    Statement(int type);
    virtual ~Statement();
    virtual void interpret(Interpreter* interpreter) = 0;
    BodyNode* body;
};
#endif
