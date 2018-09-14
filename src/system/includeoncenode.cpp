#include "includeoncenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
#include "filepermission.h"
#include "exceptionobject.h"
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

void IncludeOnceNode::test(Validator* validator, struct extras extra)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

}

Value IncludeOnceNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v = this->exp->interpret(interpreter);

    std::string absolute_path = getAbsolutePath(v.svalue);
    // Before we do anything let's make sure we have permission to load this file
    FilePermission::checkPermissionAllows(interpreter, interpreter->getCurrentScope(), absolute_path.c_str(), "r");
    interpreter->new_parented_scope();
    try
    {
        std::string script_address = v.svalue;
        if (interpreter->hasRunScript(script_address))
            return v;
        interpreter->runScript(script_address.c_str());
    }
    catch(IOException& e)
    {
        interpreter->finish_parented_scope();
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "", interpreter->getStackTraceLog());
    }
    catch(...)
    {
        interpreter->finish_parented_scope();
        throw;
    }

    interpreter->finish_parented_scope();
    return v;
}

void IncludeOnceNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
