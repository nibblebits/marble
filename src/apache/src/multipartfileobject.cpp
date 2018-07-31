
#include "multipartfileobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

MultipartFileObject::MultipartFileObject(Class* c) : Object(c)
{

}

MultipartFileObject::~MultipartFileObject()
{

}

std::shared_ptr<Object> MultipartFileObject::newInstance(Class* c)
{
    return std::make_shared<MultipartFileObject>(c);
}

Class* MultipartFileObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("MultipartFile");
    c->setDescriptorObject(std::make_shared<MultipartFileObject>(c));
    c->registerFunction("getType", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getType);
    c->registerFunction("getName", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getName);
    c->registerFunction("getPath", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getPath);
    c->registerFunction("getExtension", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getExtension);
}

void MultipartFileObject::MultipartFile_getType(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->type);
}

void MultipartFileObject::MultipartFile_getName(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->name); 
}

void MultipartFileObject::MultipartFile_getPath(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->path); 
}

void MultipartFileObject::MultipartFile_getExtension(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->ext);
}


