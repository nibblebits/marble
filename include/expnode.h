#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "statics.h"
#include "einode.h"
#include "evaluatingnode.h"
#include "validator.h"
class Interpreter;
class Value;
class ExpNode : public ExpressionInterpretableNode
{
public:
    ExpNode();
    virtual ~ExpNode();
    virtual Value interpret(Interpreter* intrerpreter, struct extras extra={});
    virtual void test(Validator* validator, struct extras extra={});
    void test_obj_access(Validator* validator, struct extras extra={});
    void test_assign(Validator* validator);
    void test_regular_exp(Validator* validator);
    bool checkShouldIgnoreExpecting(Validator* validator, ExpressionInterpretableNode* left_node, ExpressionInterpretableNode* right_node, bool no_loop=false);
    Value objectCompareOperatorOverloadExecute(Value &value1, Value &value2, std::string op, Interpreter *interpreter, bool dont_flip=false);
    Value compareGetResult(Value& value1, Value& value2, std::string op, Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    ExpressionInterpretableNode* left;
    ExpressionInterpretableNode* right;
    std::string op;
    bool isAssignmentOperator();
   
private:
    Value mathify(Value& value1, Value& value2, std::string op, Interpreter* interpreter);
};
#endif
