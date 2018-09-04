#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <vector>
#include <string>
#include <memory>
#include "variable.h"
#include "statics.h"
class Object;
class PermissionsObject;
class PermissionObject;
class Scope
{
public:
    /**
     * \param The permissions that this scope should be bound to
     */
    Scope(std::shared_ptr<PermissionsObject> permissions);
    virtual ~Scope();
    virtual void registerVariable(Variable* variable);
    Variable* createVariable();
    Variable* createVariable(std::string name, double value, MODIFIER_ACCESS modifier_access = MODIFIER_ACCESS_PUBLIC);
    Variable* createVariable(std::string name, std::string type, std::shared_ptr<Object> value, MODIFIER_ACCESS modifier_access = MODIFIER_ACCESS_PUBLIC);
    Variable* cloneCreate(Variable* variable);
    Variable* getVariable(std::string variable_name);
    Variable* getVariableAnyScope(std::string variable_name);

    /**
    * Returns the last registered variable in this singular scope.
    * \return Returns the last registered variable.
    */
    Variable* getLastRegisteredVariable();
    void removeVariable(Variable* variable);
    
    // Events
    virtual void onEnterScope(bool tell_parents);
    virtual void onLeaveScope(bool tell_parents);
    
    // Returns weather this scope is nested in the scope provided.
    bool isNestedInScope(Scope* scope);
    virtual std::vector<Variable*> getVariables();
    std::vector<Variable*> getObjectVariablesFor(std::shared_ptr<Object> object);

    std::shared_ptr<PermissionObject> getPermission(std::string name);
    std::vector<std::shared_ptr<PermissionObject>> getPermissionList(std::string name);
    
    Scope* prev;

    // The permissions for this scope, defaulted to NULL.
    std::shared_ptr<PermissionsObject> permissions;

private:
    std::vector<Variable*> variables;
    Variable* last_registered_variable;
    // Holds unique pointers for anyone that called createVariable. Memory will be freed when the scope object leaves its scope
    std::vector<std::unique_ptr<Variable>> unique_variables;
};
#endif
