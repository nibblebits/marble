#ifndef MODULE_H
#define MODULE_H
#include <string>
#include "statics.h"
class Module
{
public:
    Module(std::string name, MODULE_TYPE type);
    virtual ~Module();
    std::string getName();
    MODULE_TYPE getType();
    virtual void Init() = 0;
private:
    MODULE_TYPE type;
    std::string name;
};
#endif