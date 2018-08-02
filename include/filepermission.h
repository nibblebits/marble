#ifndef FILEPERMISSION_H
#define FILEPERMISSION_H

#include "permissionobject.h"
#include <string>
class Variable;
class SystemHandler;
class FilePermission : public PermissionObject
{
public:
    FilePermission(Class* c);
    virtual ~FilePermission();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(SystemHandler* systemHandler);

    // The location this FilePermission is bound to
    Variable* location;
    Variable* can_read;
    Variable* can_write;

    // Native FilePermission functions/methods
    static void FilePermission_SetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_GetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_SetCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_SetCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_CanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_CanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_PermissionCheck(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FilePermission_PriorAdd(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif