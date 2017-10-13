#ifndef CLASSNODE_H
#define CLASSNODE_H
#include <iostream>
#include "inode.h"
class BodyNode;
class ClassNode : public InterpretableNode
{
public:
    ClassNode();
    virtual ~ClassNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    
    std::string name;
    BodyNode* body;
    std::string parent;
};
#endif

