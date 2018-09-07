#ifndef MATHMODULE_H
#define MATHMODULE_H
#include "module.h"
class Scope;
class MathModule : public Module
{
public:
    MathModule();
    virtual ~MathModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:

};
#endif