/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "filemod_file.h"
#include "filemod.h"
#include "filepermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "misc.h"
#include "array.h"
#include "variable.h"
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

std::shared_ptr<Object> FileModule_File::newInstance(Class *c)
{
    return std::make_shared<FileModule_File>(c);
}

Class *FileModule_File::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class File
     * Responsible for File operations
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("File");
    c->setDescriptorObject(std::make_shared<FileModule_File>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        // Call the parent constructor
        object->getClass("File")->invokeObjectParentConstructor(arguments, object, interpreter);

        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        file->output = std::dynamic_pointer_cast<FileModule_FileOutputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileOutputStream")));
        file->input = std::dynamic_pointer_cast<FileModule_FileInputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileInputStream")));
        file->output->file = file->fp;
        file->input->file = file->fp;
    });

    /**
     * @class File
     * Moves the filename to the destination
     * 
     * E.g
     * move("./test.txt", "./test2.txt");
     * 
     * @works_without_class
     * function move(string filename, string dst_filename) : void
     */
    c->registerFunction("move", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), FileModule_File::File_Move);

    /**
     * @class File
     * Returns true if the given path is a file, returns false if it is a directory or an error occured
     * 
     * @works_without_class
     * function isFile(string filename) : boolean
     */
    c->registerFunction("isFile", {VarType::fromString("string")}, VarType::fromString("boolean"), FileModule_File::File_IsFile);

    /**
     * @class File
     * Attempts to open the file with the given filename. Returns true if successful otherwise false.
     * 
     * The mode provided determines how you wish to open this file.
     * Use "w" to open the file for writing this will overwrite the file if it exists
     * Use "r" to open the file for reading
     * Use "a" to open the file for appending this will create a new file if one does not exist
     * 
     * You are required a FilePermission that allows you access to the file you are trying to open along with the mode
     * function open(string filename, string mode) : void
     */
    c->registerFunction("open", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("boolean"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_Open(interpreter, arguments, return_value, object, caller_scope);
    });

    /**
     * @class File
     * Returns the output stream for this opened file allowing you to write to the file
     * function getOutputStream() : FileOutputStream
     */
    c->registerFunction("getOutputStream", {}, VarType::fromString("FileOutputStream"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->output;
    });

    /**
     * @class File
     * Returns the input stream for this opened file allowing you to read the file
     * function getInputStream() : FileInputStream
     */
    c->registerFunction("getInputStream", {}, VarType::fromString("FileInputStream"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->input;
    });

    /**
     * @class File
     * Sets the position in the file that you wish to access
     * function setPosition(number pos) : void
     */
    c->registerFunction("setPosition", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_setPosition(interpreter, arguments, return_value, object);
    });

    /**
     * @class File
     * Gets the current size of this file (not of the input stream or output stream)
     * function getSize() : number
     */
    c->registerFunction("getSize", {}, VarType::fromString("number"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_GetSize(interpreter, arguments, return_value, object);
    });

    /**
     * @class File
     * Closes this current file
     * function close() : void
     */
    c->registerFunction("close", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        File_Close(interpreter, arguments, return_value, object);
    });

    /**
     * @class File
     * 
     * Deletes the file with the specified filename throws an IOException if it does not exist or the file could not be deleted
     * 
     * function delete(string filename) : void
     */
    c->registerFunction("delete", {VarType::fromString("string")}, VarType::fromString("void"), FileModule_File::File_Delete);

    /**
     * @class File
     * 
     * Reads the entire file into memory as a string and returns the file contents.
     * Important to note that this returns only valid ASCII string data. Please use file_get_binary_contents for binary files
     * @works_without_class
     * function file_get_contents(string filename) : string
     */
    c->registerFunction("file_get_contents", {VarType::fromString("string")}, VarType::fromString("string"), FileModule_File::File_file_get_contents);
    moduleSystem->getFunctionSystem()->registerFunction("file_get_contents", {VarType::fromString("string")}, VarType::fromString("string"), FileModule_File::File_file_get_contents);

    /**
     * @class File
     * 
     * Reads the entire file into memory, returns the file contents as a number array that holds 8 bit characters in each index.
     * Use this function for reading binary files
     * @works_without_class
     * function file_get_binary_contents(string filename) : number[]
     */
    c->registerFunction("file_get_binary_contents", {VarType::fromString("string")}, VarType::fromString("number[]"), FileModule_File::File_file_get_binary_contents);
    moduleSystem->getFunctionSystem()->registerFunction("file_get_binary_contents", {VarType::fromString("string")}, VarType::fromString("number[]"), FileModule_File::File_file_get_binary_contents);


    /**
     * @class File
     * 
     * Writes the given string to the file specified by the filename.
     * If the file does not exist it creates it, if it does exist it overwrites it
     * 
     * @works_without_class
     * function file_put_contents(string filename, string data) : void
     */
    c->registerFunction("file_put_contents", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), FileModule_File::File_file_put_contents);
    moduleSystem->getFunctionSystem()->registerFunction("file_put_contents", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), FileModule_File::File_file_put_contents);


    /**
     * @class File
     * 
     * Changes the mode of the given file
     * 
     * @works_without_class
     * function chmod(string filename, number mode) : string
     */
    c->registerFunction("chmod", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("void"), FileModule_File::File_chmod);
    moduleSystem->getFunctionSystem()->registerFunction("chmod", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("void"), FileModule_File::File_chmod);

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
    if (mode != "r" && mode != "w" && mode != "a")
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Invalid mode provided: " + mode + " currently you can only read or write files for when you open a file you cannot do both together. Supported modes \"r\" = read, \"w\" = write (overwrite if it exists), \"a\" = append (create if not exists)", interpreter->getStackTraceLog());
    }
    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, mode);

    FILE *fp = fopen(filename.c_str(), mode.c_str());
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    file_obj->input->file = fp;
    file_obj->output->file = fp;
    file_obj->fp = fp;
    file_obj->filename = filename;
    return_value->dvalue = fp != NULL;
}

void FileModule_File::File_Close(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    file_obj->input->file = NULL;
    file_obj->output->file = NULL;
    int response = (file_obj->fp == NULL ? 1 : fclose(file_obj->fp));
    if (response != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to close file", interpreter->getStackTraceLog());
    file_obj->fp = NULL;
}

void FileModule_File::File_setPosition(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int response = (file_obj->fp == NULL ? 1 : fseek(file_obj->fp, values[0].dvalue, SEEK_SET));
    if (response != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to set file position", interpreter->getStackTraceLog());
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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to rename " + filename + " to " + dst_filename, interpreter->getStackTraceLog());
}

void FileModule_File::File_IsFile(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->set(isFile(values[0].svalue));
}

void FileModule_File::File_Delete(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Let's make sure we have write access so we can delete this file
    std::string filename = values[0].svalue;
    std::string abs_path = getAbsolutePath(filename);
    FilePermission::checkPermissionAllows(interpreter, caller_scope, abs_path, "w");
    if (remove(filename.c_str()) != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "The file: " + filename + " could not be deleted because it either does not exist or you lack the system permissions to do so. You do have marble permissiosn to delete this file", interpreter->getStackTraceLog());
}

void FileModule_File::File_file_get_contents(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string absolute_filename_path = getAbsolutePath(values[0].svalue);
    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, "r");
    FILE *fp = fopen(absolute_filename_path.c_str(), "r");
    if (!fp)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open the file: " + absolute_filename_path, interpreter->getStackTraceLog());

    try
    {
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char buf[size + 1];
        if (!fread(buf, size, 1, fp))
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to read the file: " + absolute_filename_path + " but it opened succesfully", interpreter->getStackTraceLog());

        // Add NULL terminator
        buf[size] = 0;
        return_value->set(std::string(buf));
        fclose(fp);
    }
    catch (...)
    {
        fclose(fp);
        throw;
    }
}

void FileModule_File::File_file_get_binary_contents(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string absolute_filename_path = getAbsolutePath(values[0].svalue);
    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, "r");
    FILE *fp = fopen(absolute_filename_path.c_str(), "r");
    if (!fp)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open the file: " + absolute_filename_path, interpreter->getStackTraceLog());

    try
    {
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        unsigned char buf[size];
        if (!fread(buf, size, 1, fp))
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to read the file: " + absolute_filename_path + " but it opened succesfully", interpreter->getStackTraceLog());

        // Ok let's create a number array to store these file contents in
        Variable* variables = new Variable[size];
        for (int i = 0; i < size; i++)
        {
            Variable *var = &variables[i];
            var->type = VARIABLE_TYPE_NUMBER;
            var->value.type = VARIABLE_TYPE_NUMBER;
            var->value.holder = var;
            var->value.dvalue = buf[i];
        }
        return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, size));
        fclose(fp);
    }
    catch (...)
    {
        fclose(fp);
        throw;
    }
}

void FileModule_File::File_file_put_contents(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string absolute_filename_path = getAbsolutePath(values[0].svalue);
    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, "w");
    FILE *fp = fopen(absolute_filename_path.c_str(), "w");
    if (!fp)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open the file: " + absolute_filename_path, interpreter->getStackTraceLog());

    try
    {
        if (!fwrite(values[1].svalue.c_str(), values[1].svalue.size(), 1, fp))
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to write the file: " + absolute_filename_path + " but it opened succesfully", interpreter->getStackTraceLog());

        fclose(fp);
    }
    catch (...)
    {
        fclose(fp);
        throw;
    }
}

void FileModule_File::File_chmod(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string absolute_filename_path = getAbsolutePath(values[0].svalue);
    // We need to make sure the scope has access to this file
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_filename_path, "r");

    // Change the permissions
    if (chmod(absolute_filename_path.c_str(), (int)values[1].dvalue) != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to change the mode of the file: " + absolute_filename_path, interpreter->getStackTraceLog());
}