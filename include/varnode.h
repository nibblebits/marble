#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include <string>
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
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    std::string getTypeAsString();
    Node* type;
    std::string name;
    InterpretableNode* value;
    
    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
    
    // The array dimensions. Zero if this is not an array
    int dimensions;
    
private:
};

#endif
