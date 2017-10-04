#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include "class.h"
#include "scope.h"
class Interpreter;
class Object : public Scope,  public std::enable_shared_from_this<Object>
{
public:
    Object(Interpreter* interpreter, Class* c);
    virtual ~Object();
    virtual void setup();
    virtual void registerVariable(Variable* variable);
    Class* getClass();
private:
    Interpreter* interpreter;
    Class* c;
};

#endif
