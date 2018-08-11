#ifndef REQUIRENODE_H
#define REQUIRENODE_H
#include "inode.h"
#include "splitter.h"
#include <string>
#include <deque>
class ExpressionInterpretableNode;
class RequireNode : public InterpretableNode
{
public:
    RequireNode();
    virtual ~RequireNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

    std::string filename;
    // The AST of the required marble source file once it has been tested
    InterpretableNode* required_ast;
private:
    Splitter splitter;
    std::string code_result;
};
#endif
