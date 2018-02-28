#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "nodes.h"
#include "singlefunction.h"
#include "vartype.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "exceptions/evaluationexception.h"
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
        try
        {
           struct Evaluation evaluation = argument_node->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
           types->push_back(evaluation.datatype);
        }
        catch(EvaluationException& ex)
        {
            throw TestError("The value provided for the function call is multityped. Ensure you cast where types differ. First type is: " + ex.type1 + " second is: " + ex.type2);
        }
    }
}

void FunctionCallNode::test(Validator* validator, struct extras extra)
{
   if (shouldIgnoreValidation())
   {
       return;
   }

   /*
    * If the accessors scope is NULL then we will default to the current validators scope
    */
   if (extra.accessors_scope == NULL)
        extra.accessors_scope = validator->getCurrentScope();

   // Lets ensure the function actually exists
   FunctionSystem* function_sys = validator->getFunctionSystem();
   std::vector<VarType> types;
   validator->useScope([&] {
        test_args(validator, &types);
    }, extra.accessors_scope);

   FunctionSystem* final_fs = NULL;
   if (!extra.is_object_exp)
      final_fs = validator->getGlobalFunctionSystem();

   if (!function_sys->hasFunction(this->name->value, types, final_fs))
   {
       if (!function_sys->hasFunction(this->name->value, final_fs))
       {
           throw TestError("The function \"" + this->name->value + "\" has not been declared");
       }
       throw TestError("The function \"" + this->name->value + "\" has not been declared that takes the given arguments");
   }
   
   if (validator->isExpecting())
   {
       // We must check the return type is valid 
       VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();
       SingleFunction* function = (SingleFunction*) function_sys->getFunctionByNameAndArguments(this->name->value, types, final_fs);
       if (function->return_type.type != expecting_type)
           throw TestError("The function " + this->name->value + " returns a " + function->return_type.value);

       if (expecting_type == VARIABLE_TYPE_OBJECT)
       {
           if (!validator->getClassSystem()->isClassInstanceOf(function->return_type.value, validator->getExpectingType()))
               throw TestError("The function " + function->name + " returns a " + function->return_type.value);
       }

   }
}

Value FunctionCallNode::interpret(Interpreter* interpreter, struct extras extra)
{
   Value value;
   std::vector<Value> argument_results;
   /*
    * If the accessors scope is NULL then we will default to the current interpreters scope.
    */
   if (extra.accessors_scope == NULL)
   {
        extra.accessors_scope = interpreter->getCurrentScope();
   }

   /* If accessing an object the current scope
    * may have become something else so we need to use the accessors scope which is where the function call took place */
   interpreter->useScope([&] {
        interpret_args(interpreter, &argument_results);
   }, extra.accessors_scope);
   

   interpreter->setLastFunctionCallNode(this);
   FunctionSystem* functionSystem = interpreter->getFunctionSystem();
   Function* function = NULL;
   if (extra.is_object_exp)
      function = functionSystem->getFunctionByName(name->value);
   else
      function = functionSystem->getFunctionByName(name->value, interpreter->getGlobalFunctionSystem());

   if (function == NULL)
   {
        Value except_value;
        except_value.set("The function with the name " + name->value + " has not been registered");
        throw SystemException(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("EntityNotRegisteredException"), {except_value}));
   }
    
   try
   {
       /**
        * If this is an object expression then the scope has already been set when the object was accessed
        */
       if (extra.is_object_exp)
       {
            function->invoke(interpreter, argument_results, &value, interpreter->getCurrentObject());
       }
       else
       {
           /* We are not in an object expression right now so it is important to use the global function system 
            * so that functions from the class(if any) are not accidently invoked by mistake. An example of this is 
            * having a print function in a class and then calling a global function that calls print. Without this
            * your version of print would be called, nasty stuff...*/
           interpreter->useFunctionSystem(interpreter->getGlobalFunctionSystem(), [&] {
               function->invoke(interpreter, argument_results, &value, interpreter->getCurrentObject());              
           });
       }
   }
   catch(SystemException& ex)
   {
       std::cout << "THROWING SYSTEM EXCEPTION" << std::endl;
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
