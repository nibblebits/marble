#include <memory>
#include "classnode.h"
#include "csystem.h"
#include "nodes.h"
#include "interpreter.h"
#include "validator.h"
#include "functionsystem.h"
#include "object.h"
#include "scope.h"

ClassNode::ClassNode() : InterpretableNode(NODE_TYPE_CLASS)
{
    this->body = NULL;
}

ClassNode::~ClassNode()
{

}

void ClassNode::test(Validator* validator)
{
    // Check to see if class already exists
    ClassSystem* class_sys = validator->getClassSystem();
    if (class_sys->hasClassWithName(name))
        throw std::logic_error("The class with the name \"" + name + "\" has already been declared");
        
    // Check to see if the parent class exists
    if (this->parent != "" && !class_sys->hasClassWithName(this->parent))
        throw std::logic_error("The parent class with the name \"" + this->parent + "\" has not been declared");
        
    Class* parent_class = class_sys->getClassByName(this->parent);
    Class* c = class_sys->registerClass(name, parent_class);
 
    // We must create a temporary object to be used with the testing process. No methods or functions will be called.
    std::shared_ptr<Object> object = std::make_shared<Object>(validator, c); 
    
    Scope* old_scope = validator->getCurrentScope();
    FunctionSystem* old_function_system = validator->getFunctionSystem();
    validator->setCurrentScope(object.get());  
    validator->setFunctionSystem(object->getClass());
    // Let's test the body of the class node
    body->onBeforeLeave([&]() -> void {
        // When leaving the body we should get all the variables that were created during testing this body and store them in the class.
        for (Variable* var : validator->getCurrentScope()->getVariables())
        {
            c->addVariable(Variable::getFromPointer(var));
        }
    });
    
    validator->beginClass(c);
    try
    {
        body->test(validator);
    } catch(std::logic_error& e)
    {
        throw std::logic_error(std::string(e.what()) + " at class " + name);
    }
    validator->endClass();
    validator->setCurrentScope(old_scope);
    validator->setFunctionSystem(old_function_system);
    
    // The class at this point holds all the variables, lets create a new object instance and add it to the validator. We will need access to this later
    object = std::make_shared<Object>(validator, c);
    validator->giveClassObject(object);
    
}

Value ClassNode::interpret(Interpreter* interpreter)
{
    ClassSystem* c_system = interpreter->getClassSystem();
    Class* parent_class = c_system->getClassByName(this->parent);
    Class* c = c_system->registerClass(name, parent_class);
    // We need to create a local scope so that we can extract variables once we are done interpreting the class body.
    std::unique_ptr<Scope> tmp_scope = std::make_unique<Scope>();
    Scope* old_scope = interpreter->getCurrentScope();
    interpreter->setCurrentScope(tmp_scope.get());
    // We will change the function system to the class so that all registered functions within the class body will become a method in the class.
    FunctionSystem* old_function_sys = interpreter->getFunctionSystem();
    interpreter->setFunctionSystem(c);
    body->apply_node_listener([&](Node* node, Value v) -> bool {
        if (node->type == NODE_TYPE_VARIABLE_DECLARATION)
        {
           Variable var = Variable::getFromPointer(v.holder);
           c->addVariable(var);
        }
        return true;
    });
    body->interpret(interpreter);
    // We are done with the body now so we can set the function system back to normal.
    interpreter->setFunctionSystem(old_function_sys);
    // Let's also reset the current scope back to the old one
    interpreter->setCurrentScope(old_scope);
    Value v;
    return v;
}

