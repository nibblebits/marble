#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include "class.h"
#include "scope.h"
class Interpreter;
class Object : public Scope
{
public:
    Object(Interpreter* interpreter, Class* c);
    virtual ~Object();
    virtual void registerVariable(Variable* variable);
    Class* getClass();
private:
    Interpreter* interpreter;
    Class* c;
};

#endif
