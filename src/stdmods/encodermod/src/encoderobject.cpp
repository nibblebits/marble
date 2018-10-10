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

#include "encoderobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "misc.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "base64/base64.h"
#include <iostream>

EncoderObject::EncoderObject(Class *c) : Object(c)
{
  
}

EncoderObject::~EncoderObject()
{
}

std::shared_ptr<Object> EncoderObject::newInstance(Class *c)
{
    return std::make_shared<EncoderObject>(c);
}

Class* EncoderObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Encoder
     * 
     * This class is responsible for everything relating to encoding. You will find routines for base64 encoding here.
     * 
     * You can use this Encoder statically. Encoder.methodName();
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("Encoder");
    c->setDescriptorObject(std::make_shared<EncoderObject>(c));
    /**
     * @class Encoder
     * 
     * Constructs this Encoder
     * function __construct(number unix_timestamp) : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Encoder
     * 
     * Base64 encodes the given string and returns the encoded result of the given value
     * 
     * @works_without_class
     * function base64_encode(string value) : string
     */
    c->registerFunction("base64_encode", {VarType::fromString("string")}, VarType::fromString("string"), EncoderObject::Encoder_base64_encode);
    moduleSystem->getFunctionSystem()->registerFunction("base64_encode", {VarType::fromString("string")}, VarType::fromString("string"), EncoderObject::Encoder_base64_encode);
    
    /**
     * @class Encoder
     * 
     * Base64 decodes the given string and returns the decoded result of the given encoded value
     * 
     * @works_without_class
     * function base64_decode(string value) : string
     */
    c->registerFunction("base64_decode", {VarType::fromString("string")}, VarType::fromString("string"), EncoderObject::Encoder_base64_decode);
    moduleSystem->getFunctionSystem()->registerFunction("base64_decode", {VarType::fromString("string")}, VarType::fromString("string"), EncoderObject::Encoder_base64_decode);

}

void EncoderObject::Encoder_base64_encode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   return_value->set(base64_encode((const unsigned char*) values[0].svalue.c_str(), values[0].svalue.size()));
}


void EncoderObject::Encoder_base64_decode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   return_value->set(base64_decode(values[0].svalue.c_str()));
}

