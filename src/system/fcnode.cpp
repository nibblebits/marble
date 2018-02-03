#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "nodes.h"
#include "singlefunction.h"
#include "vartype.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include <iostream>
#include <stdexcept>
FunctionCallNode::FunctionCallNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION_CALL)
{
    this->name = NULL;
}

FunctionCallNode::~FunctionCallNode()
{

}

void FunctionCallNode::test_args(Validator* validator, std::vector<VarType>* types)
{
    for (ExpressionInterpretableNode* argument_node : this->arguments)
    {
        argument_node->test(validator);
        struct Evaluation evaluation = argument_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        types->push_back(evaluation.datatype);
    }
}

void FunctionCallNode::test(Validator* validator)
{
   if (shouldIgnoreValidation())
   {
       return;
   }

   // Lets ensure the function actually exists
   FunctionSystem* function_sys = validator->getFunctionSystem();
   std::vector<VarType> types;
   if (validator->isAccessingObject())
   {
        validator->useScope([&] {
            test_args(validator, &types);
        }, validator->getAccessorsScope());
   }
   else
   {
       test_args(validator, &types);
   }

   if (!function_sys->hasFunction(this->name->value, types))
   {
       throw TestError("The function \"" + this->name->value + "\" has not been declared that takes the given arguments");
   }
   
   if (validator->isExpecting())
   {
       // We must check the return type is valid 
       VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();
       SingleFunction* function = (SingleFunction*) function_sys->getFunctionByNameAndArguments(this->name->value, types);
       if (function->return_type.type != expecting_type)
           throw TestError("The function " + this->name->value + " returns a " + function->return_type.value);

       if (expecting_type == VARIABLE_TYPE_OBJECT)
       {
           if (!validator->getClassSystem()->isClassInstanceOf(function->return_type.value, validator->getExpectingType()))
               throw TestError("The function " + function->name + " returns a " + function->return_type.value);
       }

   }
}


Value FunctionCallNode::interpret(Interpreter* interpreter)
{
   Value value;
   std::vector<Value> argument_results;
   if (interpreter->isAccessingObject())
   {
      interpreter->useScope([&] {
        interpret_args(interpreter, &argument_results);
      }, interpreter->getAccessorsScope());
   }
   else
   {
       interpret_args(interpreter, &argument_results);
   }
   interpreter->setLastFunctionCallNode(this);
   FunctionSystem* functionSystem = interpreter->getFunctionSystem();
   Function* function = functionSystem->getFunctionByName(name->value);
   if (function == NULL)
   {
        Value except_value;
        except_value.set("The function with the name " + name->value + " has not been registered");
        throw SystemException(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("EntityNotRegisteredException"), {except_value}));
   }
   
   try
   {
       function->invoke(interpreter, argument_results, &value, interpreter->getCurrentObject());
   }
   catch(SystemException& ex)
   {
       throw ex;
   }
   
   return value;
}

void FunctionCallNode::interpret_args(Interpreter* interpreter, std::vector<Value>* argument_results)
{
    for (ExpressionInterpretableNode* argument_node : this->arguments)
    {
        Value v = argument_node->interpret(interpreter);
        argument_results->push_back(v);
    }
}

void FunctionCallNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        FunctionSystem* function_sys = handler->getFunctionSystem();
        std::vector<VarType> types;
        for (ExpressionInterpretableNode* argument_node : this->arguments)
        {
           struct Evaluation evaluation = argument_node->evaluate(handler, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
           types.push_back(evaluation.datatype);
        }
        SingleFunction* function = (SingleFunction*) function_sys->getFunctionByNameAndArguments(this->name->value, types);
        if (function == NULL)
            throw std::logic_error("The function call to evaluate has a call to a function that does not exist");
            
        evaluation->datatype = function->return_type;
        
    }
}
