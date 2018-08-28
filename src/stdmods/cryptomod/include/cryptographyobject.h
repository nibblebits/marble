#ifndef CRYPTOGRAPHY_OBJECT
#define CRYPTOGRAPHY_OBJECT

#include "object.h"
#include <jsoncpp/json/json.h>
#include <map>
#include <string>


class ModuleSystem;
class CryptographyObject : public Object
{
public:
    CryptographyObject(Class* c);
    virtual ~CryptographyObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // Native Cryptography methods
    static void Cryptography_sha256(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Cryptography_sha1(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif