#ifndef CLASS_H
#define CLASS_H
#include <string>
#include "functionsystem.h"
class Class : public FunctionSystem
{
public:
    Class(std::string name, FunctionSystem* interpreter);
    virtual ~Class();
    std::string name;
};
#endif
