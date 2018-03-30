#ifndef FILEMOD_FILE
#define FILEMOD_FILE

#include <memory>
#include <stdio.h>
#include "filemod_fileoutputstream.h"
#include "filemod_fileinputstream.h"
#include "object.h"
#include "module.h"

class FileModule_File : public Object
{
public:
    FileModule_File(Class* c);
    virtual ~FileModule_File();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);

    FILE* fp;
    std::string filename;
    std::shared_ptr<FileModule_FileOutputStream> output;
    std::shared_ptr<FileModule_FileInputStream> input;

private:
    // Native File functions/methods
    static void File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_setPosition(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void File_GetSize(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};

#endif