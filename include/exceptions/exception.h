#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
class Exception : public std::logic_error
{
public:
    Exception(std::string message) : std::logic_error(message) {}
    std::string getMessage() { return std::string(what()); }
    virtual ~Exception() throw() {}
};

#endif
