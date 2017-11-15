#include "trynode.h"
#include "interpreter.h"
#include "object.h"
#include "class.h"
#include "nodes.h"
#include "exceptions/systemexception.h"
#include "exceptions/testerror.h"

TryNode::TryNode() : InterpretableNode(NODE_TYPE_TRY)
{
    this->try_body = NULL;
    this->catch_varnode = NULL;
    this->catch_body = NULL;
}

TryNode::~TryNode()
{

}

void TryNode::test(Validator* validator)
{
    this->try_body->test(validator);
    this->catch_varnode->test(validator);
    if(!this->catch_varnode->isObject())
        throw TestError("Primitive types are not allowed for exceptions");
    if (this->catch_varnode->isArray())
        throw TestError("The exception catch variable must not be an array");
    if (this->catch_varnode->value != NULL)
        throw TestError("Exception variables must not hold a default value");
   
    ClassSystem* c_system = validator->getClassSystem();
    struct Evaluation evaluation = this->catch_varnode->evaluate(validator, EVALUATION_TYPE_DATATYPE);
    Class* c = c_system->getClassByName(evaluation.datatype.value);
    Class* exception_c = c_system->getClassByName("Exception");
    if (!c->instanceOf(exception_c))
    {
        throw TestError("The class " + c->name + " is not an instance of the class Exception. Exceptions must extend the Exception class");
    }
         
    this->catch_body->test(validator);
}

Value TryNode::interpret(Interpreter* interpreter)
{
    try
    {
        this->try_body->interpret(interpreter);
    } catch(SystemException& ex)
    {
        std::shared_ptr<Object> thrown_object = ex.getObject();
        // Let's see if we can actually catch this, if not we will rethrow the exception
        Class* thrown_exception_class = thrown_object->getClass();
        struct Evaluation evaluation = this->catch_varnode->evaluate(interpreter, EVALUATION_TYPE_DATATYPE);
        Class* exception_class = interpreter->getClassSystem()->getClassByName(evaluation.datatype.value);
        if (!thrown_exception_class->instanceOf(exception_class))
        {
            // Ok the exception that was thrown is not an instance of our defined exception class so lets just rethrow it so it can be dealt with else where
            throw SystemException(thrown_object);
        }
        
        // Let's create an exception value that will point to the exception object        
        Value exception_value;
        exception_value.type = VALUE_TYPE_OBJECT;
        exception_value.ovalue = thrown_object;
        
        
        // Let's interpret the exception variable node and then set the value to the exception's object value that was thrown.
        Value v = this->catch_varnode->interpret(interpreter);
        v.holder->value.set(&exception_value);
        
        // We now must interpret the catch body
        this->catch_body->interpret(interpreter);
    }
    
    Value v;
    return v;
}

void TryNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of try nodes is not supported");
}

