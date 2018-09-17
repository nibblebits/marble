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

#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H
#include <string.h>
#include <memory>
#include <vector>
#include <functional>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "scopehandler.h"
#include "logger.h"
#include "statics.h"

class Class;
class Scope;
struct current_object
{
    std::shared_ptr<Object> object;
    Class* c;
};

class SystemHandler : public ScopeHandler
{
public:
    SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem, SYSTEM_HANDLER_RULES rules=0);
    virtual ~SystemHandler();
    void setFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();

    /**
     * Calls the call provided with the current function system being set to the f_system provided. After the call is complete
     * the functon system before this function call is restored.
     * 
     * \param f_system The FunctionSystem to use
     * \param call The lambda function call that should be called
     */
    void useFunctionSystem(FunctionSystem* f_system, std::function<void()> call);

    void useScopeAndFunctionSystem(Scope* scope, FunctionSystem* f_system, std::function<void()> call);

    /**
    * Gets the last known Object that was accessed. This object will be the value of the object that currently a method is being called on or an attribute being accessed on.. E.g obj.function();
    */
    std::shared_ptr<Object> getCurrentObject();

    /**
     *  Returns the current object class.
     */
    Class* getCurrentObjectClass();

    /**
    * Used to set the current Object that is currently being accessed. This object must be the value of the object that currently a method is being called on or an attribute being accessed on.. E.g obj.function();
    * @param object The current object that is being accessed
    * @param c The class of this object that is being accessed
    */
    void setCurrentObject(std::shared_ptr<Object> object, Class* c);
    /**
    * Should be called upon finishing access to an object
    */
    void finishCurrentObject();


    /**
     * Returns true if their is an object being accessed
     * Expression:
     * return getCurrentObject() != NULL
     */
     bool isAccessingObject();

    FunctionSystem* getGlobalFunctionSystem();
    FunctionSystem* getFunctionSystem();
    FunctionSystem* getBaseFunctionSystem();
    ClassSystem* getBaseClassSystem();
    ClassSystem* getClassSystem();
    
    SYSTEM_HANDLER_TYPE getType();


protected:


    // The base class system is where the systems classes from native libraries are registered
    ClassSystem* baseClassSystem;
    ClassSystem classSystem;
    // The base function system is where the systems functions from native libraries are registered
    FunctionSystem* baseFunctionSystem;
    FunctionSystem globalFunctionSystem;
    FunctionSystem* currentFunctionSystem;
    Logger logger;
    SYSTEM_HANDLER_TYPE type;
    Scope* current_calling_scope;
    std::vector<Scope*> calling_scopes;
    std::vector<struct current_object> current_obj_stack;
    struct current_object current_obj;
};
#endif
