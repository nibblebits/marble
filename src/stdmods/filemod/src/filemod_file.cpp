#include "filemod_file.h"
#include "exceptions/systemexception.h"

FileModule_File::FileModule_File(Class* c) : Object(c)
{
    this->fp = NULL;
    this->output = NULL;
}

FileModule_File::~FileModule_File()
{

}


Class* FileModule_File::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("File");
    c->setDescriptorObject(std::make_shared<FileModule_File>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        // Call the parent constructor
        object->invokeParentConstructor(interpreter, arguments);
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        file->output = std::dynamic_pointer_cast<FileModule_FileOutputStream>(Object::create(moduleSystem->getClassSystem()->getClassByName("FileOutputStream")));
        file->output->file = file;
    });
    c->registerFunction("open", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        File_Open(interpreter, arguments, return_value, object);
    });
    c->registerFunction("getOutputStream", {}, VarType::fromString("FileOutputStream"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        std::shared_ptr<FileModule_File> file = std::dynamic_pointer_cast<FileModule_File>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = file->output;
    });
    c->registerFunction("close", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
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
    return_value->dvalue = fp != NULL;
}

void FileModule_File::File_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<FileModule_File> file_obj = std::dynamic_pointer_cast<FileModule_File>(object);
    int response = fclose(file_obj->fp);
    if (response != 0)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("IOException")));
}
