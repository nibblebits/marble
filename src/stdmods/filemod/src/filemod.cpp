#include "filemod.h"
#include "object.h"
#include <sstream>
#include <time.h>
#include <stdio.h>


FileModule_File::FileModule_File(Class* c) : Object(c)
{
    this->fp = NULL;
    this->output = std::make_shared<CommonModule_OutputStream>(c);
}

FileModule_File::~FileModule_File()
{

}

std::shared_ptr<Object> FileModule_File::newInstance(Class* c)
{
    return std::make_shared<FileModule_File>(c);
}

FileModule::FileModule() : Module("filemod", "File Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

FileModule::~FileModule()
{

}

void FileModule::Init()
{
    log("File Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("File");
    c->setDescriptorObject(std::make_shared<FileModule_File>(c));
    c->registerFunction("open", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        File_Open(interpreter, arguments, return_value, object);
    });
    c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        File_Print(interpreter, arguments, return_value, object);
    });
    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void FileModule::newInterpreter(Interpreter* interpreter)
{
  
}

// Native IO functions/methods
void FileModule::File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    return_value->type = VALUE_TYPE_NUMBER;
    std::string filename = values[0].svalue;
    std::string mode = values[1].svalue;
    FILE* fp = fopen(filename.c_str(), mode.c_str());
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    file_obj->fp = fp;
    return_value->dvalue = fp != NULL;
}

void FileModule::File_Print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    return_value->type = VALUE_TYPE_NUMBER;
    std::string value = values[0].svalue;
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int r = fwrite(value.c_str(), 1, value.size(), file_obj->fp);
    return_value->dvalue = r != 0;
}