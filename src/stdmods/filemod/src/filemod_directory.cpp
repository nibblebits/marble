#include "filemod_directory.h"
#include "misc.h"
#include "filemod.h"
#include "interpreter.h"

#include <sys/stat.h>
#include <sys/types.h>

FileModule_Directory::FileModule_Directory(Class* c) : Object(c)
{

}

FileModule_Directory::~FileModule_Directory()
{

}

std::shared_ptr<Object> FileModule_Directory::newInstance(Class* c)
{
    return std::make_shared<Object>(c);
}

Class* FileModule_Directory::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Directory");
    c->setDescriptorObject(std::make_shared<FileModule_Directory>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        // Blank constructor
    });

    c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString("boolean"), FileModule_Directory::Directory_Create);

}


/**
 * Called when a new interpreter is running
*/
void FileModule_Directory::newInterpreter(Interpreter* interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Directory", "Directory", Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Directory"), {}));
}

void FileModule_Directory::Directory_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Do we have permission for this?
    std::string abs_path = getAbsolutePath(values[0].svalue);
    FileModule::permissionCheck(interpreter, caller_scope, abs_path, "w");
    
    // Create the directory and return weather it was ok or not
    return_value->set(mkdir(values[0].svalue.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
}