#ifndef COMMONMODULE_OUTPUTSTREAM
#define COMMONMODULE_OUTPUTSTREAM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
class CommonModule_OutputStream : public Object
{
public:
    CommonModule_OutputStream(Class* c);
    virtual ~CommonModule_OutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);
};


#endif