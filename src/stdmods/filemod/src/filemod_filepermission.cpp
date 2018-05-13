#include "filemod_filepermission.h"
#include "modulesystem.h"
#include "function.h"
#include "misc.h"
FileModule_FilePermission::FileModule_FilePermission(Class* c) : PermissionObject(c)
{
    this->location = "/";
    this->can_read = false;
    this->can_write = false;
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
    file_permission_cls->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), Function::Blank);
    /*
    * Sets the location this FilePermission should be bound to
    * function setLocation(string location) : void;
    */
    file_permission_cls->registerFunction("setLocation", {VarType::fromString("string")}, VarType::fromString("void"), FileModule_FilePermission::FilePermission_SetLocation);

    /*
     * Sets weather this FilePermission should allow reading from the location that this FilePermission is bound to
     * function setCanRead(boolean can_read) : void;
     */

    file_permission_cls->registerFunction("setCanRead", {VarType::fromString("boolean")}, VarType::fromString("void"), FileModule_FilePermission::FilePermission_SetCanRead); 
    /*
    * Sets weather this FilePermission should allow writing to the location that this FilePermission is bound to
    * function setCanWrite(boolean can_write) : void;
    */
    file_permission_cls->registerFunction("setCanWrite", {VarType::fromString("boolean")}, VarType::fromString("void"), FileModule_FilePermission::FilePermission_SetCanWrite);

}

void FileModule_FilePermission::FilePermission_SetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    file_perm_obj->location = getAbsolutePath(values[0].svalue);
}

void FileModule_FilePermission::FilePermission_GetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    return_value->set(file_perm_obj->location);
}

void FileModule_FilePermission::FilePermission_SetCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    file_perm_obj->can_read = (bool)values[0].dvalue;
}

void FileModule_FilePermission::FilePermission_SetCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    file_perm_obj->can_write = (bool)values[0].dvalue;
}

void FileModule_FilePermission::FilePermission_CanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    return_value->set(file_perm_obj->can_read);    
}

void FileModule_FilePermission::FilePermission_CanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileModule_FilePermission> file_perm_obj = std::dynamic_pointer_cast<FileModule_FilePermission>(object);
    return_value->set(file_perm_obj->can_write);    
}