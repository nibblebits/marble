#include "filemod_fileinputstream.h"
#include "filemod.h"
#include "exceptions/systemexception.h"
#include <stdio.h>
#include <iostream>

FileModule_FileInputStream::FileModule_FileInputStream(Class* c) : CommonModule_InputStream(c)
{

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
    Class* c = c_system->registerClass("FileInputStream", c_system->getClassByName("InputStream"));
    c->setDescriptorObject(std::make_shared<FileModule_FileInputStream>(c));
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
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("IOException")));
    
    // Let's get the FileModule_File and get the native FILE* and write this buffer
    std::shared_ptr<FileModule_File> file_obj = stream->file;
    FILE* file = file_obj->fp;

    int amount = (int)arguments[0].dvalue;
    char buf[amount];
    int amount_read = fread(buf, 1, amount, file);
    if (amount_read == 0)
    {
        // A read error has occured if we are not at the end of the file
        if (!feof(file))
            throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("IOException")));
    }
    for (int i = 0; i < amount_read; i++)
    {
        stream->buffer.push_back(buf[i]);
    }
}