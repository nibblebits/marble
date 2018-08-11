#include "filemod_file.h"
#include "filemod.h"
#include "filepermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "misc.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

FileModule_File::FileModule_File(Class *c) : Object(c)
{
    this->fp = NULL;
    this->output = NULL;
    this->input = NULL;
}

FileModule_File::~FileModule_File()
{
    // Let's close the file if it wasen't closed automatically
    if (this->fp != NULL)
        fclose(this->fp);
}


Class *FileModule_File::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("File");
    c->setDescriptorObject(std::make_shared<FileModule_File>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        // Call the parent constructor
        object->getClass("File")->invokeObjectParentConstructor(arguments, object, interpreter);

        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        file->output = std::dynamic_pointer_cast<FileModule_FileOutputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileOutputStream")));
        file->input = std::dynamic_pointer_cast<FileModule_FileInputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileInputStream")));
        file->output->file = file;
        file->input->file = file;
    });

    /**
     * Moves the filename to the destination
     * 
     * function move(string filename, string dst_filename) : void
     */
    c->registerFunction("move", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), FileModule_File::File_Move);

    /**
     * Returns true if the given path is a file, returns false if it is a directory or an error occured
     * 
     * function isFile() : boolean
     */
    c->registerFunction("isFile", {VarType::fromString("string")}, VarType::fromString("boolean"), FileModule_File::File_IsFile);

    c->registerFunction("open", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("boolean"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_Open(interpreter, arguments, return_value, object, caller_scope);
    });
    c->registerFunction("getOutputStream", {}, VarType::fromString("FileOutputStream"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->output;
    });

    c->registerFunction("getInputStream", {}, VarType::fromString("FileInputStream"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->input;
    });

    c->registerFunction("setPosition", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_setPosition(interpreter, arguments, return_value, object);
    });

    c->registerFunction("getSize", {}, VarType::fromString("number"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_GetSize(interpreter, arguments, return_value, object);
    });

    c->registerFunction("close", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_Close(interpreter, arguments, return_value, object);
    });
    return c;
}

void FileModule_File::newInterpreter(Interpreter *interpreter)
{
    // Create a variable called File of type of File this is used to give the illusion of static methods
    interpreter->getRootScope()->createVariable("File", "File", Object::create(interpreter, interpreter->getClassSystem()->getClassByName("File"), {}));
}

// Native IO functions/methods
void FileModule_File::File_Open(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->type = VALUE_TYPE_NUMBER;
    std::string filename = values[0].svalue;
    std::string mode = values[1].svalue;

    std::string absolute_filename_path = getAbsolutePath(filename);

    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, mode);

    FILE *fp = fopen(filename.c_str(), mode.c_str());
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);

    file_obj->fp = fp;
    file_obj->filename = filename;
    return_value->dvalue = fp != NULL;
}

void FileModule_File::File_Close(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int response = (file_obj->fp == NULL ? 1 : fclose(file_obj->fp));
    if (response != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))));
}

void FileModule_File::File_setPosition(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int response = (file_obj->fp == NULL ? 1 : fseek(file_obj->fp, values[0].dvalue, SEEK_SET));
    if (response != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))));
}

void FileModule_File::File_GetSize(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    struct stat stat_buf;
    int rc = stat(file_obj->filename.c_str(), &stat_buf);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = rc == 0 ? stat_buf.st_size : -1;
}

void FileModule_File::File_Move(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string filename = values[0].svalue;
    std::string dst_filename = values[1].svalue;
    if (rename(filename.c_str(), dst_filename.c_str()) != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to rename " + filename + " to " + dst_filename);
}

void FileModule_File::File_IsFile(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set(isFile(values[0].svalue));
}