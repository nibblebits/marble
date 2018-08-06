#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include <string>
#include "node.h"
#include "token.h"
#include "statics.h"
#include "node.h"
#include "evaluatingnode.h"
#include "inode.h"
class ExpressionInterpretableNode;
class VarNode : public InterpretableNode
{
public:
    VarNode();
    virtual ~VarNode();
    virtual void test(Validator* validator, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});

    /**
     * Attempts to interpret and handle operator overloading assignments
     * if this is invalid for this assignment it returns false otherwise true
     */
    bool handleOperatorOverloadIfValid(Interpreter* interpreter, std::string type_str, Variable* variable, ExpressionInterpretableNode* value_node);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    std::string getTypeAsString();
    bool isArray();
    bool isPrimitive();
    bool isObject();
    EvaluatingNode* type;
    std::string name;
    InterpretableNode* value;
    
    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
    
    // The array dimensions. Zero if this is not an array
    int dimensions;
    
private:
};

#endif
