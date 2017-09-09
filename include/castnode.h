#ifndef CASTNODE_H
#define CASTNODE_H

#include "einode.h"
class CastNode : public ExpressionInterpretableNode
{
public:
    CastNode();
    virtual ~CastNode();
    virtual Value interpret(Interpreter* interpreter);
    Node* casting_to;
    ExpressionInterpretableNode* to_cast;
private:
    std::string get_string_value(Value* from);
    double get_double_value(Value* from);
};
#endif
