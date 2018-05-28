#ifndef CLASSNODE_H
#define CLASSNODE_H
#include <iostream>
#include "inode.h"
class BodyNode;
class Class;
class ClassNode : public InterpretableNode
{
public:
    ClassNode();
    virtual ~ClassNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

    std::string name;
    BodyNode* body;
    std::string parent;
    bool is_pure;
    bool is_final;
protected:
    void test_variables(Validator* validator, std::shared_ptr<Object> object, Class* c, struct extras* extra);
};
#endif

