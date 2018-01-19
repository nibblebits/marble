#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "node.h"
#include "einode.h"
#include "value.h"
#include <vector>
class IdentifierNode;
class Interpreter;
class FunctionCallNode : public ExpressionInterpretableNode
{
public:
    FunctionCallNode();
    virtual ~FunctionCallNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    /**
    *
    * Interprets the function arguments for this function call and then returns a pointer to the function that should be called.
    */
    Function* interpret_args_get_function();
    
    /**
    *
    * Used when interpreting methods. The local_scope provided is the local scope of the method
    * caller and should be provided. The interpreters current scope should be changed by the programmer
    * before calling this method and it should be changed to the scope of the class object.
    */
    Value interpret(Interpreter* interpreter, Scope* local_scope);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    IdentifierNode* name;
    std::vector<ExpressionInterpretableNode*> arguments;
};
#endif
