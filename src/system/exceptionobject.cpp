#include "exceptionobject.h"
#include "interpreter.h"
#include "posinfo.h"
#include "thrownode.h"

ExceptionObject::ExceptionObject(Class* c) : Object(c)
{

}

ExceptionObject::~ExceptionObject()
{

}
    
void ExceptionObject::setStackLog(std::vector<struct stack_log_part> stack_log)
{
    this->stack_log = stack_log;
}

void ExceptionObject::setThrowNode(ThrowNode* throwNode)
{
    this->throwNode = throwNode;
}


std::shared_ptr<Object> ExceptionObject::newInstance(Class* c)
{
    return std::make_shared<ExceptionObject>(c);
}

std::string ExceptionObject::getStackTrace()
{
    std::string trace = "Thrown " + getClass()->name + "@" + this->throwNode->posInfo.filename + ": " + std::to_string(this->throwNode->posInfo.line) + "\n";
    for (int i = this->stack_log.size()-1; i >= 0; i--)
    {
        struct stack_log_part part = this->stack_log[i];
        trace += "at " + part.function_name + "@" + part.posInfo.filename + ": " + std::to_string(part.posInfo.line) + "\n";
    }
    return trace;
}
