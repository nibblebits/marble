#include "exceptions/systemexception.h"
SystemException::SystemException(std::shared_ptr<Object> object)
{
    this->object = object;
}

SystemException::~SystemException()
{

}

std::shared_ptr<Object> SystemException::getObject()
{
    return this->object;
}
