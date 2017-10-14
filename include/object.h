#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include "class.h"
#include "scope.h"
class SystemHandler;
class Object : public Scope,  public std::enable_shared_from_this<Object>
{
public:
    Object(SystemHandler* sys_handler, Class* c);
    virtual ~Object();
    virtual void registerVariable(Variable* variable);
    Class* getClass();
    virtual void onEnterScope();
    virtual void onLeaveScope();
private:
    SystemHandler* sys_handler;
    Class* c;
};

#endif
