#ifndef FILEPERMISSION_H
#define FILEPERMISSION_H

#include "permissionobject.h"
#include <string>
class ModuleSystem;
class FileModule_FilePermission : public PermissionObject
{
public:
    FileModule_FilePermission(Class* c);
    virtual ~FileModule_FilePermission();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static void registerClass(ModuleSystem* moduleSystem);

    // The location this FilePermission is bound to
    std::string location;
    bool can_read;
    bool can_write;

    // Native FilePermission functions/methods
    static void FilePermission_SetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_GetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_SetCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_SetCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_CanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_CanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};
#endif