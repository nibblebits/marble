#include "filemod_file.h"
#include "exceptions/systemexception.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

FileModule_File::FileModule_File(Class* c) : Object(c)
{
    this->fp = NULL;
    this->output = NULL;
}

FileModule_File::~FileModule_File()
{
    // Let's close the file if it wasen't closed automatically
    if (this->fp != NULL)
        fclose(this->fp);
}


Class* FileModule_File::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("File");
    c->setDescriptorObject(std::make_shared<FileModule_File>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        // Call the parent constructor
        object->invokeParentConstructor(interpreter, arguments);
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        file->output = std::dynamic_pointer_cast<FileModule_FileOutputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileOutputStream")));
        file->input = std::dynamic_pointer_cast<FileModule_FileInputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileInputStream")));
        file->output->file = file;
        file->input->file = file;
    });
    c->registerFunction("open", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        File_Open(interpreter, arguments, return_value, object);
    });
    c->registerFunction("getOutputStream", {}, VarType::fromString("FileOutputStream"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->output;
    });
    c->registerFunction("getInputStream", {}, VarType::fromString("FileInputStream"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->input;
    });
    c->registerFunction("getSize", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        File_GetSize(interpreter, arguments, return_value, object);
    });
    c->registerFunction("close", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        File_Close(interpreter, arguments, return_value, object);
    });
    return c;
}
// Native IO functions/methods
void FileModule_File::File_Open(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    return_value->type = VALUE_TYPE_NUMBER;
    std::string filename = values[0].svalue;
    std::string mode = values[1].svalue;
    FILE* fp = fopen(filename.c_str(), mode.c_str());
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    file_obj->fp = fp;
    file_obj->filename = filename;
    return_value->dvalue = fp != NULL;
}

void FileModule_File::File_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int response = fclose(file_obj->fp);
    if (response != 0)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("IOException")));
}

void FileModule_File::File_GetSize(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    struct stat stat_buf;
    int rc = stat(file_obj->filename.c_str(), &stat_buf);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = rc == 0 ? stat_buf.st_size : -1;
}