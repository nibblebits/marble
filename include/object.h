#ifndef OBJECT_H
#define OBJECT_H
#include "class.h"
class Object
{
public:
    Object(Class* c);
    virtual ~Object();
    Class* getClass();
private:
    Class* c;
};

#endif
