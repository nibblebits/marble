#include "fnode.h"
#include "nodes.h"
#include "statics.h"
#include "fnode.h"
#include "functionsystem.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
FunctionNode::FunctionNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION)
{
    this->body = NULL;
    this->return_type = NULL;
    this->is_pure = false;
}

FunctionNode::~FunctionNode()
{

}

void FunctionNode::test(Validator* validator, struct extras extra)
{
    std::vector<VarType> var_types;
    validator->new_parented_scope();
    for (VarNode* node : this->args)
    {
        node->test(validator);
        struct Evaluation evaluation = node->type->evaluate(validator, EVALUATION_TYPE_DATATYPE);
        var_types.push_back(evaluation.datatype);
    }   
        
    FunctionSystem* func_sys = validator->getFunctionSystem();
    
    if(func_sys->hasFunctionLocally(this->name, var_types))
        throw TestError("The function with the name \"" + this->name + "\" has already been registered with the same arguments provided");
    if (is_pure)
    {
        // We are a pure method lets make sure we are in a class and that our class is pure
        if (!validator->isInClass())
        {
            throw TestError("Functions may only be marked as pure within classes but the function with the name \"" + this->name + " is marked pure without a class");
        }

        if (!validator->getCurrentClass()->is_pure)
        {
            throw TestError("The function with the name \"" + this->name + "\" is defined pure. However the class that it is in is not.");
        }
    }
    // Let's add the function to the list
    Function* function = func_sys->registerFunction(this);
    if (!is_pure)
    {
        func_sys->setCurrentFunction(function);
        this->body->test(validator);
        func_sys->finishCurrentFunction();
    }
    validator->finish_parented_scope();
}

Value FunctionNode::interpret(Interpreter* interpreter, struct extras extra)
{
    interpreter->getFunctionSystem()->registerFunction(this);
    Value v;
    return v;
}

void FunctionNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluating of function nodes is not supported");
}

