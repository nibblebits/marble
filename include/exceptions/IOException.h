#ifndef IO_ERROR_H
#define IO_ERROR_H

#include "exception.h"
class IOException : public Exception
{
public:
    IOException(std::string message) : Exception(message) {}
};

#endif
