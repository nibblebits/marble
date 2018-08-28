#include "cryptomod.h"
#include "cryptographyobject.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

CryptoModule::CryptoModule() :  Module("cryptomod", "Cryptography Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

CryptoModule::~CryptoModule()
{

}

void CryptoModule::Init()
{
    // Register the Cryptography class
    CryptographyObject::registerClass(this->getModuleSystem());
}

void CryptoModule::newInterpreter(Interpreter *interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Cryptography", "Cryptography", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Cryptography"), {}));
}