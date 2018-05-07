#include "filemod_filepermission.h"
#include "modulesystem.h"
#include "function.h"
FileModule_FilePermission::FileModule_FilePermission(Class* c) : PermissionObject(c)
{

}

FileModule_FilePermission::~FileModule_FilePermission()
{


}


std::shared_ptr<Object> FileModule_FilePermission::newInstance(Class* c)
{
    return std::make_shared<FileModule_FilePermission>(c);
}

void FileModule_FilePermission::registerClass(ModuleSystem* moduleSystem)
{
    Class* file_permission_cls = moduleSystem->getClassSystem()->registerClass("FilePermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    file_permission_cls->setDescriptorObject(std::make_shared<FileModule_FilePermission>(file_permission_cls));

    file_permission_cls->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
}