#ifndef FILEMOD_FILEOUTPUTSTREAM_H
#define FILEMOD_FILEOUTPUTSTREAM_H
#include "module.h"
#include "../../../commonmod/include/commonmod.h"
#include <memory>
class FileModule_FileOutputStream : public CommonModule_OutputStream
{
public:
    FileModule_FileOutputStream(Class* c);
    virtual ~FileModule_FileOutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);
};

#endif