#ifndef FILEMOD_DIRECTORY
#define FILEMOD_DIRECTORY

#include <memory>
#include <string>
#include <stdio.h>
#include "filepermission.h"
#include "object.h"
#include "module.h"


/**
 * Responsible for directory related conditions such as creating and deleting directories
 */
class Interpreter;
class FileModule_Directory : public Object
{
public:
    FileModule_Directory(Class* c);
    virtual ~FileModule_Directory();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);


    /**
     * Called when a new interpreter is running
     */
    static void newInterpreter(Interpreter* interpreter);


private:
    // Native Directory functions/methods
    static void Directory_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Directory_List(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif