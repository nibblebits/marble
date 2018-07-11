#ifndef FILEMOD_FILE
#define FILEMOD_FILE

#include <memory>
#include <string>
#include <stdio.h>
#include "filemod_filepermission.h"
#include "filemod_fileoutputstream.h"
#include "filemod_fileinputstream.h"
#include "object.h"
#include "module.h"

class Interpreter;
class Scope;
class FileModule_File : public Object
{
public:
    FileModule_File(Class* c);
    virtual ~FileModule_File();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);


    /**
     * Called when a new interpreter is running
     */
    static void newInterpreter(Interpreter* interpreter);

    FILE* fp;
    std::string filename;
    std::shared_ptr<FileModule_FileOutputStream> output;
    std::shared_ptr<FileModule_FileInputStream> input;

private:
    // Native File functions/methods
    static void File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void File_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_setPosition(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_GetSize(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_Move(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif