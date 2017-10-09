#ifndef CASTNODE_H
#define CASTNODE_H

#include "einode.h"
class Validator;
class CastNode : public ExpressionInterpretableNode
{
public:
    CastNode();
    virtual ~CastNode();
    virtual Value interpret(Interpreter* interpreter);
    virtual void test(Validator* validator);
    Node* casting_to;
    ExpressionInterpretableNode* to_cast;
private:
    std::string get_string_value(Value* from);
    double get_double_value(Value* from);
};
#endif
