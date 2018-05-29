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
    /**
     * Registers functions for the test validation process this is required so that
     * classes can reference any function regardless if its below them or not in the class body
     * 
     * Later after this function we then test the bodies of all the functions in our classes function system
     */
    void register_functions_for_test(Validator* validator, std::shared_ptr<Object> object, Class* c, struct extras* extra);
};
#endif

