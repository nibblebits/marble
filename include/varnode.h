#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include "node.h"
#include "token.h"
#include "statics.h"
#include "node.h"
#include "inode.h"
class VarNode : public InterpretableNode
{
public:
    VarNode();
    virtual ~VarNode();
    virtual Value interpret(Interpreter* interpreter);
    Node* type;
    std::string name;
    Node* value;
    
    // The array dimensions. Zero if this is not an array
    int dimensions;
    
private:
};

#endif
