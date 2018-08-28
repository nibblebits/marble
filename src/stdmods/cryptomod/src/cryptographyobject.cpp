#include "../include/cryptographyobject.h"
#include "../include/sha256.h"
#include "../include/sha1.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "misc.h"
#include "class.h"
#include "array.h"
#include <iostream>
#include <thread>

CryptographyObject::CryptographyObject(Class *c) : Object(c)
{
}

CryptographyObject::~CryptographyObject()
{
}

std::shared_ptr<Object> CryptographyObject::newInstance(Class *c)
{
    return std::make_shared<CryptographyObject>(c);
}

void CryptographyObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Cryptography extends Object
     * 
     * This is the Cryptography class it is responsible for all things cryptography including encryption and hashing
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Cryptography");
    c->setDescriptorObject(std::make_shared<CryptographyObject>(c));

    /**
     * @class Cryptography
     * 
     * Constructs this Cryptography object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Cryptography
     * 
     * Hashes the given string with the sha256 algorithm.
     * Returned is the hashed representation of the given string
     * 
     * @alias sha256 use sha256("string") as an alias if you choose rather than Cryptography.sha256
     * 
     * function sha256(string input) : string
     */
    c->registerFunction("sha256", {VarType::fromString("string")}, VarType::fromString("string"), CryptographyObject::Cryptography_sha256);
    /**
     * 
     * Hashes the given string with the sha256 algorithm.
     * Returned is the hashed representation of the given string
     * 
     * @alias Cryptography.sha256
     * function sha256(string input) : string
     */
    moduleSystem->getFunctionSystem()->registerFunction("sha256", {VarType::fromString("string")}, VarType::fromString("string"), CryptographyObject::Cryptography_sha256);

    /**
     * @class Cryptography
     * 
     * Hashes the given string with the sha1 algorithm.
     * Returned is the hashed representation of the given string
     * 
     * @alias sha1 use sha1("string") as an alias if you choose rather than Cryptography.sha1
     * 
     * function sha1(string input) : string
     */
    c->registerFunction("sha1", {VarType::fromString("string")}, VarType::fromString("string"), CryptographyObject::Cryptography_sha1);
    /**
     * 
     * Hashes the given string with the sha1 algorithm.
     * Returned is the hashed representation of the given string
     * 
     * @alias Cryptography.sha1
     * function sha1(string input) : string
     */
    moduleSystem->getFunctionSystem()->registerFunction("sha1", {VarType::fromString("string")}, VarType::fromString("string"), CryptographyObject::Cryptography_sha1);

}


void CryptographyObject::Cryptography_sha256(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set(sha256(values[0].svalue));
}

void CryptographyObject::Cryptography_sha1(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set(sha1(values[0].svalue));
}
