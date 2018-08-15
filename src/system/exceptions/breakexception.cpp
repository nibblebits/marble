#include "exceptions/breakexception.h"
BreakException::BreakException(BREAK_TYPE type)
{
    this->type = type;
}

BreakException::~BreakException()
{

}
    