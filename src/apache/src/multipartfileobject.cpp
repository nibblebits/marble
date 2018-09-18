
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
#include "multipartfileobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

MultipartFileObject::MultipartFileObject(Class *c) : Object(c)
{
}

MultipartFileObject::~MultipartFileObject()
{
}

std::shared_ptr<Object> MultipartFileObject::newInstance(Class *c)
{
    return std::make_shared<MultipartFileObject>(c);
}

Class *MultipartFileObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class MultipartFile
     * 
     * Holds file information for an uploaded file
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("MultipartFile");
    c->setDescriptorObject(std::make_shared<MultipartFileObject>(c));
    /**
     * @class MultipartFile
     * Returns the file type for this uploaded file for example: image/jpeg
     * function getType() : string
     */
    c->registerFunction("getType", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getType);
    /**
     * @class MultipartFile
     * Returns the filename of this file that the web client sent
     * function getName() : string
     */
    c->registerFunction("getName", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getName);
    /**
     * @class MultipartFile
     * Returns the full path to this uploaded file. You should move this file from this path to a path in your
     * web application
     * function getPath() : string
     */
    c->registerFunction("getPath", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getPath);
    /**
     * @class MultipartFile
     * Returns the extension of this file that the web client sent
     * function getExtension() : string
     */
    c->registerFunction("getExtension", {}, VarType::fromString("string"), MultipartFileObject::MultipartFile_getExtension);
}

void MultipartFileObject::MultipartFile_getType(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->type);
}

void MultipartFileObject::MultipartFile_getName(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->name);
}

void MultipartFileObject::MultipartFile_getPath(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->path);
}

void MultipartFileObject::MultipartFile_getExtension(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->ext);
}
