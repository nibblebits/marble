#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "interpreter.h"
#include <stdexcept>
#include <iostream>
SystemException::SystemException(std::shared_ptr<ExceptionObject> object)
{
    if (object == NULL)
        throw std::logic_error("A null object was provided");
    this->object = object;
}

SystemException::SystemException(std::shared_ptr<ExceptionObject> object, std::string message, std::vector<struct stack_log_part> log_parts) : SystemException(object)
{
    this->object->setMessage(message);
    this->object->setStackLog(log_parts);
}

SystemException::~SystemException()
{

}

std::shared_ptr<ExceptionObject> SystemException::getObject()
{
    return this->object;
}
