#include "inode.h"

InterpretableNode::InterpretableNode(int type) : EvaluatingNode(type)
{
    this->ignore_validation = false;
}

InterpretableNode::~InterpretableNode()
{

}

void InterpretableNode::test(Validator* validator, struct extras extra)
{
   
}

void InterpretableNode::ignoreValidation()
{
    this->ignore_validation = true;
}

bool InterpretableNode::shouldIgnoreValidation()
{
    return this->ignore_validation;
}