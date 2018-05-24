#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <stdexcept>
#include <iostream>
SystemException::SystemException(std::shared_ptr<ExceptionObject> object)
{
    if (object == NULL)
        throw std::logic_error("A null object was provided");
    this->object = object;
}

SystemException::SystemException(std::shared_ptr<ExceptionObject> object, std::string message) : SystemException(object)
{
    this->object->setMessage(message);
}
SystemException::~SystemException()
{

}

std::shared_ptr<ExceptionObject> SystemException::getObject()
{
    return this->object;
}
