#include "includenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
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

void IncludeNode::test(Validator* validator)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

}

Value IncludeNode::interpret(Interpreter* interpreter)
{
    Value v;
    v = this->exp->interpret(interpreter);
    try
    {
        interpreter->runScript(v.svalue.c_str());
    }
    catch(IOException& e)
    {
        std::cout << e.getMessage() << std::endl;
        throw SystemException(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {}));
    }
    return v;
}

void IncludeNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
