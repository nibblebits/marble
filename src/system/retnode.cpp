#include "retnode.h"
#include "einode.h"
#include "functionsystem.h"
#include "writtenfunction.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
#include <iostream>

ReturnNode::ReturnNode() : InterpretableNode(NODE_TYPE_RETURN)
{
    this->exp = NULL;
}

ReturnNode::~ReturnNode()
{

}

void ReturnNode::test(Validator* validator, struct extras extra)
{
    FunctionSystem* function_system = validator->getFunctionSystem();
    if (function_system->isInFunction())
    {
        SingleFunction* function = (SingleFunction*) function_system->getCurrentFunction();
        if (this->exp != NULL)
        {
            struct Evaluation evaluation = this->exp->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
            if (evaluation.datatype != function->return_type)
                throw TestError("Invalid return expression for the given return type for function \"" + function->name + "\"");
        }
    }
}

Value ReturnNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    if (this->exp != NULL)
        v = this->exp->interpret(interpreter);
    FunctionSystem* function_system = interpreter->getFunctionSystem();
    if (function_system->isInFunction())
    {
        WrittenFunction* current_function = (WrittenFunction*) function_system->getCurrentFunction();
        current_function->return_node = this;
        current_function->return_value = v;
    }
    return v;
}

void ReturnNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    this->exp->evaluate(handler, expected_evaluation);
}
