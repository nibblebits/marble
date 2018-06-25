#ifndef TIMEOUTEXCEPTION_H
#define TIMEOUTEXCEPTION_H

#include "exception.h"
class TimeoutException : public Exception
{
public:
    TimeoutException(std::string message) : Exception(message) {}
};

#endif
