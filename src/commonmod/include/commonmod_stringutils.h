#ifndef COMMONMODULE_STRINGUTILS
#define COMMONMODULE_STRINGUTILS
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
class CommonModule_StringUtils : public Object
{
public:
    CommonModule_StringUtils(Class* c);
    virtual ~CommonModule_StringUtils();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);
};


#endif