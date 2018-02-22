#ifndef COMMONMODULE_OUTPUTSTREAM
#define COMMONMODULE_OUTPUTSTREAM
#include "object.h"
#include <stdio.h>
#include <string>
class CommonModule_OutputStream : public Object
{
public:
    CommonModule_OutputStream(Class* c);
    virtual ~CommonModule_OutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
};


#endif