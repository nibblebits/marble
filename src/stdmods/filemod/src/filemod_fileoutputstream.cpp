#include <iostream>
#include "filemod_fileoutputstream.h"
FileModule_FileOutputStream::FileModule_FileOutputStream(Class* c) : CommonModule_OutputStream(c)
{

}

FileModule_FileOutputStream::~FileModule_FileOutputStream()
{

}

std::shared_ptr<Object> FileModule_FileOutputStream::newInstance(Class* c)
{
    return std::make_shared<FileModule_FileOutputStream>(c);
}

Class* FileModule_FileOutputStream::registerClass(ModuleSystem* moduleSystem)
{
    ClassSystem* c_system = moduleSystem->getClassSystem();
    Class* c = c_system->registerClass("FileOutputStream", c_system->getClassByName("OutputStream"));
    c->setDescriptorObject(std::make_shared<FileModule_FileOutputStream>(c));
    Function* f = c->registerFunction("flush", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
 
    });
    return c;
}