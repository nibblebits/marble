#ifndef NODE_H
#define NODE_H
#include <memory>
#include "posinfo.h"
class Node
{
public:
    Node(int type);
    virtual ~Node();
    int getType();
    Node* next;
    int const type;
    PosInfo posInfo;
};

#endif
