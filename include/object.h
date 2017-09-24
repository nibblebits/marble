#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include "class.h"
#include "scope.h"
class Object
{
public:
    Object(Class* c);
    virtual ~Object();
    Class* getClass();
    Scope* getScope();
private:
    Class* c;
    std::unique_ptr<Scope> obj_scope;
};

#endif
