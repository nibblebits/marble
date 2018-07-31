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
