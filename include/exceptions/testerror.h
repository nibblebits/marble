#ifndef TESTERROR_H
#define TESTERROR_H

#include "exception.h"
class TestError : public Exception
{
public:
    TestError(std::string message) : Exception(message) {}
};

#endif
