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

#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <vector>
#include "variable.h"
#include "functionsystem.h"
#include "typedef_func.h"
#include "value.h"


class Function;
class Object;
class Interpreter;
class Class : public FunctionSystem
{
public:
    Class(SystemHandler* sys_handler, std::string name, FunctionSystem* prev_fc_sys, std::shared_ptr<Object> descriptor_obj);
    Class(SystemHandler* sys_handler, std::string name, Class* parent, std::shared_ptr<Object> descriptor_obj);
    virtual ~Class();
    virtual Function* registerFunction(std::string name, std::vector<VarType> args, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint);
    virtual Function* registerFunction(FunctionNode* fnode);
    void addVariable(Variable v);
    bool hasVariableWithName(std::string name);
    Variable getVariable(std::string name);
    std::vector<Variable> getVariables();
    
    std::vector<Function*> getOverloadedOperatorFunctions();
    std::vector<Function*> getOverloadedOperatorFunctions(std::string op);
    bool hasOverloadedOperator(std::string op, std::string argument1);
    bool hasOverloadedOperator(std::string op, std::string argument1, std::string argument2);
    bool hasOverloadedOperator(std::string op);

    Class* getClassWhoHasVariable(std::string name);
    void setDescriptorObject(std::shared_ptr<Object> object);
    std::shared_ptr<Object> getDescriptorObject();

    /**
     * Invokes the parent constructor of this class for the given object and provides the values provided.
     * \param values The values to pass to the parent constructor
     * \param object The Object to invoke the constructor on
     * \param interpreter The Interpreter at the time of calling this function
     * 
     * \attention Ensure that the parent Class does have a constructor registered
     */
    void invokeObjectParentConstructor(std::vector<Value> values, std::shared_ptr<Object> object, Interpreter* interpreter);
    
    bool instanceOf(std::string class_name);
    bool instanceOf(Class* c);
    std::string name;
    Class* parent;
    bool is_pure;
    bool is_final;
    
private:
    std::shared_ptr<Object> descriptor_obj;
    std::vector<Variable> local_variables;
};
#endif
