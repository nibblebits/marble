#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include "node.h"
#include "token.h"
#include "statics.h"
class VarNode : public Node
{
public:
    VarNode();
    virtual ~VarNode();
    Node* type;
    std::string name;
    Node* value;
    
    // The array dimensions. Zero if this is not an array
    int dimensions;
private:
};

#endif
