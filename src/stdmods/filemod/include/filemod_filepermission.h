#ifndef FILEPERMISSION_H
#define FILEPERMISSION_H

#include "permissionobject.h"
class ModuleSystem;
class FileModule_FilePermission : public PermissionObject
{
public:
    FileModule_FilePermission(Class* c);
    virtual ~FileModule_FilePermission();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static void registerClass(ModuleSystem* moduleSystem);

};
#endif