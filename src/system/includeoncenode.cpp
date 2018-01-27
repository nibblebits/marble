#include "includeoncenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
#include "exceptions/systemexception.h"
#include "exceptions/IOException.h"
#include "object.h"
#include "misc.h"

#include <iostream>
IncludeOnceNode::IncludeOnceNode() : InterpretableNode(NODE_TYPE_INCLUDE_ONCE)
{

}

IncludeOnceNode::~IncludeOnceNode()
{

}

void IncludeOnceNode::test(Validator* validator)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

}

Value IncludeOnceNode::interpret(Interpreter* interpreter)
{
    Value v;
    v = this->exp->interpret(interpreter);
    try
    {
        std::string script_address = v.svalue;
        if (interpreter->hasRunScript(script_address))
            return v;
        interpreter->runScript(script_address.c_str());
    }
    catch(IOException& e)
    {
        std::cout << e.getMessage() << std::endl;
        throw SystemException(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {}));
    }
    return v;
}

void IncludeOnceNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
