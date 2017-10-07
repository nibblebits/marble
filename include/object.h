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
    virtual void registerVariable(Variable* variable);
    Class* getClass();
    virtual void onEnterScope();
    virtual void onLeaveScope();
private:
    Interpreter* interpreter;
    Class* c;
};

#endif
