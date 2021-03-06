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

#include "validator.h"
#include "nodes.h"
#include "logger.h"
#include "object.h"
#include "class.h"
#include "interpreter.h"
#include "csystem.h"
#include "debug.h"
#include <iostream>
#include "exceptions/testerror.h"
Validator::Validator(Logger* logger, Interpreter* interpreter) : SystemHandler(SYSTEM_HANDLER_VALIDATOR, interpreter->getClassSystem(), interpreter->getFunctionSystem())
{
    this->interpreter = interpreter;
    this->logger = logger;
    this->current_class = NULL;
    
    ClassSystem* c_system = interpreter->getClassSystem();
    // We must create class objects for all current classes so that they will be compatible with the validation system
    for (Class* c : c_system->getAllClasses())
    {
        giveClassObject(Object::create(c));
    }
    
    // It is important that our default object descriptor matches that of the main class system.
    std::shared_ptr<Object> default_obj_descriptor = c_system->getDefaultObjectDescriptor();
    if (!default_obj_descriptor)
        throw std::logic_error("The base class system must have a non NULL object descriptor before initializing a validator");
        
    getClassSystem()->setDefaultObjectDescriptor(c_system->getDefaultObjectDescriptor());

    
}

Validator::~Validator()
{
}


void Validator::giveClassObject(std::shared_ptr<Object> object)
{
    if (object == NULL)
        throw std::logic_error("You must provide a non NULL object to the Validator when giving a class object. giveClassObject");
    this->class_objects.push_back(object);
}

bool Validator::isInClass()
{
    return this->current_class != NULL;
}

std::shared_ptr<Object> Validator::getClassObject(std::string name)
{
    for (std::shared_ptr<Object> obj : this->class_objects)
    {
        if (obj->getClass()->name == name)
            return obj;
    }

    return NULL;
}


void Validator::validate(Node* root_node)
{
    InterpretableNode* current_node = (InterpretableNode*) root_node;
    while(current_node != NULL)
    {
        try
        {
            current_node->test(this);
        } catch(TestError& ex)
        {
            this->logger->error(ex.what(), current_node->posInfo);
        }
        
        current_node = (InterpretableNode*) current_node->next;
    }
}

Interpreter* Validator::getInterpreter()
{
    return this->interpreter;
}

void Validator::beginClass(Class* current_class)
{
    if (this->current_class != NULL)
        throw std::logic_error("A class has already begun. End your own class: Validator::endClass();");
    this->current_class = current_class;
}

void Validator::endClass()
{
    if (this->current_class == NULL)
        throw std::logic_error("No class has been begun. You must begin a class with: Validator::beginClass(); before calling Validator::endClass();");
    this->current_class = NULL;
}

Class* Validator::getCurrentClass()
{
    return this->current_class;
}

void Validator::ignoreClass(std::string class_name)
{
    if (isClassIgnored(class_name))
        return;
    this->ignored_classes.push_back(class_name);
}

bool Validator::isClassIgnored(std::string class_name)
{
    for (std::string c : this->ignored_classes)
    {
        if (c == class_name)
            return true;
    }

    return false;
}


void Validator::save()
{
    this->rules_stack.push_back(this->rules);
    struct rules r;
    this->rules = r;
}

void Validator::restore()
{
    this->rules = this->rules_stack.back();
    this->rules_stack.pop_back();
}


void Validator::expectingArray(int dimensions)
{
    if (!isExpecting())
        throw std::logic_error("You must start expecting a type before specifying the array dimensions");

    this->rules.expected_array_dimensions = dimensions;
}

void Validator::expecting(std::string type)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    this->rules.expecting_type = type;
    this->rules.expecting_value_type = Value::getValueTypeForString(type);
    this->rules.expecting_variable_type = Variable::getVariableTypeForString(type);
}

bool Validator::isExpecting()
{
    return this->rules.expecting_type != "";
}

bool Validator::isExpectingArray()
{
    return this->rules.expected_array_dimensions > 0;
}

int Validator::getExpectedArrayDimensions()
{
    return this->rules.expected_array_dimensions;
}

void Validator::endExpecting()
{
    this->rules.expecting_type = "";
    this->rules.expected_array_dimensions = 0;
}

std::string Validator::getExpectingType()
{
    return this->rules.expecting_type;
}

VALUE_TYPE Validator::getExpectingValueType()
{
    return this->rules.expecting_value_type;
}

VARIABLE_TYPE Validator::getExpectingVariableType()
{
    return this->rules.expecting_variable_type;
}


