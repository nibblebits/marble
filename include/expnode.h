#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "statics.h"
#include "einode.h"
#include "validator.h"
class ExpNode : public ExpressionInterpretableNode
{
public:
    ExpNode();
    virtual ~ExpNode();
    virtual Value interpret(Interpreter* intrerpreter);
    virtual void test(Validator* validator);
    
    ExpressionInterpretableNode* left;
    ExpressionInterpretableNode* right;
    std::string op;
    bool isAssignmentOperator();
   
private:
    Value mathify(Value& value1, Value& value2, std::string op);
};
#endif
