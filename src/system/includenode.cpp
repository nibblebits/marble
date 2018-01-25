#include "includenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
#include "exceptions/systemexception.h"
#include "object.h"

#include <iostream>
IncludeNode::IncludeNode() : InterpretableNode(NODE_TYPE_INCLUDE)
{
    this->interpreted = false;
}

IncludeNode::~IncludeNode()
{

}

void IncludeNode::test(Validator* validator)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

    // We must interpret the include node now so that validation can find any functions or variables that are included
    interpret(validator->getInterpreter());
}

Value IncludeNode::interpret(Interpreter* interpreter)
{
    Value v;
    // If we have already interpreted this then lets return.
    if (this->interpreted)
        return v;

    this->interpreted = true;
    v = this->exp->interpret(interpreter);
    try
    {
        interpreter->runScript(v.svalue.c_str());
    }
    catch(...)
    {
        throw SystemException(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {}));
    }
    return v;
}

void IncludeNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
