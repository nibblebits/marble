#ifndef FILEMODULE_H
#define FILEMODULE_H
#include "module.h"
#include "../../commonmod/include/commonmod.h"
#include "object.h"
#include <stdio.h>
#include <string>

class FileModule_File : public Object
{
public:
    FileModule_File(Class* c);
    virtual ~FileModule_File();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    FILE* fp;
    std::string filename;
    std::shared_ptr<Object> output;
};

class FileModule : public Module
{
public:
    FileModule();
    virtual ~FileModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:
    // Native Time functions/methods
    void File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    void File_Print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};
#endif