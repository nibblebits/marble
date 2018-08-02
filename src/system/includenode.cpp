#include "includenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "exceptions/IOException.h"
#include "object.h"

#include <iostream>
IncludeNode::IncludeNode() : InterpretableNode(NODE_TYPE_INCLUDE)
{

}

IncludeNode::~IncludeNode()
{

}

void IncludeNode::test(Validator* validator, struct extras extra)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

}

Value IncludeNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v = this->exp->interpret(interpreter);
    interpreter->new_parented_scope();
    try
    {
        if (interpreter->isNestedScript(v.svalue))
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InfiniteLoopException"), {})));
        }
        
        interpreter->runScript(v.svalue.c_str());
    }
    catch(SystemException& ex)
    {
        interpreter->finish_parented_scope();
        throw ex;
    }
    catch(IOException& e)
    {
        interpreter->finish_parented_scope();
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})));
    }
    interpreter->finish_parented_scope();
    return v;
}

void IncludeNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
