#ifndef CLASS_H
#define CLASS_H
#include <string>
#include "functionsystem.h"
class Class : public FunctionSystem
{
public:
    Class(std::string name, FunctionSystem* interpreter);
    Class(std::string name, Class* parent);
    virtual ~Class();
    std::string name;
    Class* parent;
};
#endif
