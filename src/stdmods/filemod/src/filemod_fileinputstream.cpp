#include "filemod_fileinputstream.h"
#include "filemod.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <stdio.h>
#include <iostream>

FileModule_FileInputStream::FileModule_FileInputStream(Class* c) : CommonModule_InputStream(c)
{
    this->file = NULL;
}

FileModule_FileInputStream::~FileModule_FileInputStream()
{

}

std::shared_ptr<Object> FileModule_FileInputStream::newInstance(Class* c)
{
    return std::make_shared<FileModule_FileInputStream>(c);
}
Class* FileModule_FileInputStream::registerClass(ModuleSystem* moduleSystem)
{
    ClassSystem* c_system = moduleSystem->getClassSystem();
    /**
     * class FileInputStream extends InputStream
     * 
     * Responsible for reading from files. Input streams can be retrieved when opening files
     */
    Class* c = c_system->registerClass("FileInputStream", c_system->getClassByName("InputStream"));
    c->setDescriptorObject(std::make_shared<FileModule_FileInputStream>(c));
    /**
     * @class FileInputStream
     * 
     * Fills the input stream buffer with the provided number of bytes or until the file reaches the end
     * function fill(number amount) : void
     */
    Function* f = c->registerFunction("fill", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        FileInputStream_Fill(interpreter, arguments, return_value, object);
    });
    return c;
}


void FileModule_FileInputStream::FileInputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_FileInputStream> stream = std::dynamic_pointer_cast<FileModule_FileInputStream>(object);

    /* If this input stream does not belong to a file then there is nothing we can do so throw an exception. 
     * This will happen when a programmer creates a FileOutputStream manually within marble code*/
    if (stream->file == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Stream not bounded to file");
    
    // Let's get the native FILE* and write this buffer
    FILE* file = stream->file;

    int amount = (int)arguments[0].dvalue;
    char buf[amount];
    int amount_read = fread(buf, 1, amount, file);
    if (amount_read == 0)
    {
        // A read error has occured if we are not at the end of the file
        if (!feof(file))
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to read from file");
    }
    for (int i = 0; i < amount_read; i++)
    {
        stream->buffer.push_back(buf[i]);
    }
}