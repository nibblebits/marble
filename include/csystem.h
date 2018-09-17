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

#ifndef CLASSSYSTEM_H
#define CLASSSYSTEM_H
#include <vector>
#include <memory>
#include <string>
#include "class.h"

class SystemHandler;
/** 
 * The ClassSystem is in charge of maintaining classes this includes registering and retrieving classes.
 * The ClassSystem can also have previous class systems. This is required so when a new SystemHandler is created all previously registered classes
 * do not have to be lost. You just pass the class system in the constructor of the SystemHandler. 
 * When retrieving a class from the ClassSystem with the getClassByName() method if the class is not found the ClassSystem will check the previous
 * class systems if it has any.
 * \attention Each ClassSystem is independant from any others. By setting a previous class system it does not mean that both these ClassSystems will share properties they will not. This means that if one ClassSystem has a default base class of Object and another has a default base class of Foo both these base classes will remain the same upon setting a previous class system. This also means that the SystemHandler will remain the same for both ClassSystems.
 **/
class ClassSystem
{
public:
    ClassSystem();
    virtual ~ClassSystem();
    void setSystemHandler(SystemHandler* sys_handler);
    void setPreviousClassSystem(ClassSystem* prev_sys);
    /**
    * Sets the default object descriptor Object that will become the default Object instance for all classes that are registered from the time of calling this method.
    * All classes with a NULL object descriptor upon calling this method will have their object descriptor set to this one.
    * Classes will have NULL object descriptors if the object descriptor has not been set yet and classes have still been registered.
    */
    void setDefaultObjectDescriptor(std::shared_ptr<Object> defaultObjectDescriptor);
    std::shared_ptr<Object> getDefaultObjectDescriptor();
    ClassSystem* getPreviousClassSystem();
    void setDefaultBaseClass(Class* c);

    /**
     * Gets the default base class for this ClassSystem. If none was found
     * then the parents default base class is used.
     */
    Class* getDefaultBaseClass();
    Class* registerClass(std::string class_name, Class* parent=NULL, CLASS_REGISTER_RULES rules=0);
    Class* getClassByName(std::string name);
    bool hasClassWithName(std::string name);
    std::vector<Class*> getAllClasses();
    std::string getAllClassesAsString();

    // Helper functions

    /**
     * Returns true if class1 == class2 or class1 is an instance of class2
     */
    bool isClassInstanceOf(std::string class1, std::string class2);
private:
    std::vector<std::unique_ptr<Class>> classes;
    std::shared_ptr<Object> defaultObjectDescriptor;
    ClassSystem* prev_sys;
    SystemHandler* sys_handler;
    Class* defaultBaseClass;
};

#endif
