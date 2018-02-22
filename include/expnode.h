#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "statics.h"
#include "einode.h"
#include "evaluatingnode.h"
#include "validator.h"
class ExpNode : public ExpressionInterpretableNode
{
public:
    ExpNode();
    virtual ~ExpNode();
    virtual Value interpret(Interpreter* intrerpreter, struct extras extra);
    virtual void test(Validator* validator, struct extras extra);
    void test_obj_access(Validator* validator);
    void test_assign(Validator* validator);
    void test_regular_exp(Validator* validator);

    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    ExpressionInterpretableNode* left;
    ExpressionInterpretableNode* right;
    std::string op;
    bool isAssignmentOperator();
   
private:
    Value mathify(Value& value1, Value& value2, std::string op);
};
#endif
