#ifndef EXCEPTIONOBJECT_H
#define EXCEPTIONOBJECT_H

#include <memory>

#include "object.h"

class Interpreter;
class ThrowNode;
class ExceptionObject : public Object
{
public:
    ExceptionObject(Class* c);
    virtual ~ExceptionObject();
    
    /**
    * Sets the stack log for this exception. This will be used to generate the stack trace when calling getStackTrace()*
    */
    void setStackLog(std::vector<struct stack_log_part> stack_log);
    /**
    
    * Used to set the throw node that threw this exception object. */
    void setThrowNode(ThrowNode* throwNode);
    
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    /**
    * Gets a stack trace based on the stack log provided
    */
    std::string getStackTrace();
private:
    std::vector<struct stack_log_part> stack_log;
    ThrowNode* throwNode;
};

#endif
