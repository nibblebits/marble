#include <memory>
#include "classnode.h"
#include "csystem.h"
#include "nodes.h"
#include "interpreter.h"
#include "validator.h"
#include "functionsystem.h"
#include "object.h"
#include "scope.h"
#include "extras.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptions/testerror.h"
#include "function.h"
#include "singlefunction.h"
#include "writtenfunction.h"

ClassNode::ClassNode() : InterpretableNode(NODE_TYPE_CLASS)
{
    this->body = NULL;
    this->is_pure = false;
    this->is_final = false;
}

ClassNode::~ClassNode()
{

}


void ClassNode::test_variables(Validator* validator, std::shared_ptr<Object> object, Class* c, struct extras* extra)
{

    object->runThis([&]() { 

        /**
         * We only want to test variable declarations at this point as we are testing class variables
         */
        body->apply_node_listener([&](Node* node) -> NODE_LISTENER_ACTION {
            if (node->type == NODE_TYPE_VARIABLE_DECLARATION)
            {
                // This is a variable declaration let's carry on and test it
               return NODE_LISTENER_ACTION_CARRY_ON;
            }
        
            // This is not a variable declaration so ignore it, we only care about variables here
            return NODE_LISTENER_ACTION_IGNORE_NODE;
            
        });

        body->onAfterTestNode([&](Node* node) {
            // We have just tested the variable declaration let's add the last registered object variable to our class variables
            c->addVariable(Variable::getFromPointer(object->getLastRegisteredVariable()));
        });

        try
        {
            /* By testing the body of the class a new scope will be created. 
             * we do not want this as we want the class object scope to be the scope of the object.
             * if we do not instruct the body node to keep our scope then a new parented scope will be created
             * and the "this" variable will then not work from within the object.*/
            body->test(validator, KEEP_SCOPE);
        } catch(TestError& e)
        {
            throw TestError(std::string(e.what()) + " at class " + name);
        }
        
    }, validator, c, OBJECT_ACCESS_TYPE_CLASS_SCAN);

}

void ClassNode::register_functions_for_test(Validator* validator, std::shared_ptr<Object> object, Class* c, struct extras* extra)
{
     object->runThis([&]() { 
        // We only care about functions here so skip everything else
        body->apply_node_listener([&](Node* node) -> NODE_LISTENER_ACTION {
            // We want to ignore all nodes that are not functions as we are only interested in testing functions
            if (node->type != NODE_TYPE_FUNCTION)
            {
                return NODE_LISTENER_ACTION_IGNORE_NODE;
            }
            
            FunctionNode* f_node = (FunctionNode*) node;
            // Test the function node but only the declaration this will then cause it to be added to the class
            struct extras extra;
            extra.state = EXTRA_STATE_FUNCTION_DECLARATION_ONLY;
            f_node->test(validator, extra);

            // We should now also ignore this function node as we have dealt with it ourselves and do not want the BodyNode testing it again
            return NODE_LISTENER_ACTION_IGNORE_NODE;
        });

        try
        {
            /* By testing the body of the class a new scope will be created. 
             * we do not want this as we want the class object scope to be the scope of the object.
             * if we do not instruct the body node to keep our scope then a new parented scope will be created
             * and the "this" variable will then not work from within the object.*/
            body->test(validator, KEEP_SCOPE);
        } catch(TestError& e)
        {
            throw TestError(std::string(e.what()) + " at class " + name);
        }
    }, validator, c, OBJECT_ACCESS_TYPE_CLASS_SCAN);
}

void ClassNode::test(Validator* validator, struct extras extra)
{
    // Check to see if class already exists
    ClassSystem* class_sys = validator->getClassSystem();
    if (class_sys->hasClassWithName(name))
        throw TestError("The class with the name \"" + name + "\" has already been declared");
        
    // Check to see if the parent class exists
    if (this->parent != "" && !class_sys->hasClassWithName(this->parent))
        throw TestError("The parent class with the name \"" + this->parent + "\" has not been declared");
        
    Class* parent_class = class_sys->getClassByName(this->parent);
    Class* c = class_sys->registerClass(name, parent_class);
    if (is_pure)
        c->is_pure = true;

    if (is_final)
        c->is_final = true;
 
    // Set to true if constructors with arguments should be rejected
    bool zero_arg_constructors_only = false;

     /* Permissions may only have non argument constructors 
     * The reason for this is because when creating a class that extends Permission
     * an object of that type is created and added to the current scope permissions so that the creator of the permission class
     * automatically inherits the permissions. It is not possible to know what to pass to the constructor as this is not a function call
     * therefore only constructors with zero arguments are valid.*/
    if (c->instanceOf("Permission"))
    {
        zero_arg_constructors_only = true;
    }

    // We must create a temporary object to be used with the testing process. No methods or functions will be called.
    std::shared_ptr<Object> object = Object::create(c); 
    validator->giveClassObject(object);
    validator->beginClass(c);
    // Test all variables and register them to our object. This is required so that variables can be accessed from any position in the class
    this->test_variables(validator, object, c, &extra);
    // Register all the functions in our class ready for testing later. This is required so that functions in classes can resolve other functions regardless of position
    this->register_functions_for_test(validator, object, c, &extra);
    
    // Finally we need to now test the function's properly including function body's
    object->runThis([&]() { 
        for (Function* func: c->getFunctions())
        {
            // We only care about functions that were written in marble in this class as that's what we want to use
            if (func->type == FUNCTION_TYPE_WRITTEN)
            {
                WrittenFunction* w_func = (WrittenFunction*) func;
                FunctionNode* f_node = (FunctionNode*) w_func->fnode;

                // Test the function and allow functions to already exist as the function will exist due to the register_functions_for_test function we called early
                struct extras extra;
                extra.state = EXTRA_STATE_ALLOW_FUNCTION_TO_EXIST;
                f_node->test(validator, extra);

                // If we only allow zero argument constructors for this class then we need to ensure that this function node follows this requirement
                if (zero_arg_constructors_only)
                {
                    if (f_node->name == "__construct" && !f_node->args.empty())
                    {
                        throw TestError("Constructors that have arguments are not allowed for the class with the name: " + name);
                    }
                }
            }
        }
    }, validator, c, OBJECT_ACCESS_TYPE_CLASS_SCAN);

    
    // Finally let's now test the function bodies of all functions that will now be registered in our class
    validator->endClass();

    if (parent_class != NULL)
    {
        if (parent_class->is_final)
        {
            throw TestError("The class " + parent_class->name + " is final and cannot be extended");
        }
        
        if (!is_pure && parent_class->is_pure)
        {
            // Our parent is pure so we need to check this class has overrided the pure methods in the parent class
            for (Function* f : parent_class->getPureFunctions())
            {
                SingleFunction* sf = (SingleFunction*) f;
                // This is fine for now but it needs to be changed to also check for arguments. As now multiple pure functions with the same name wont be validated correctly.
                if (!c->hasFunctionLocally(f->name, sf->argument_types))
                {
                    throw TestError("The class " + c->name + " extends " + parent_class->name + " but has not implemented the pure function " + f->name);
                }
            }
        }
    }
}

Value ClassNode::interpret(Interpreter* interpreter, struct extras extra)
{
    ClassSystem* c_system = interpreter->getClassSystem();
    Class* parent_class = c_system->getClassByName(this->parent);
    Class* c = c_system->registerClass(name, parent_class);
    if (is_pure)
        c->is_pure = true;
    if (is_final)
        c->is_final = true;


    #warning Best to refactor below
    // We need to create a local scope so that we can extract variables once we are done interpreting the class body.
    Scope* old_scope = interpreter->getCurrentScope();
    std::unique_ptr<Scope> tmp_scope = std::unique_ptr<Scope>(new Scope(NULL));
    interpreter->setCurrentScope(tmp_scope.get());
    // We will change the function system to the class so that all registered functions within the class body will become a method in the class.
    FunctionSystem* old_function_sys = interpreter->getFunctionSystem();
    interpreter->setFunctionSystem(c);
    body->onAfterInterpretNode([&](Node* node, Value v) -> bool {
        if (node->type == NODE_TYPE_VARIABLE_DECLARATION)
        {
           Variable var = Variable::getFromPointer(v.holder);
           c->addVariable(var);
        }
    });
    body->interpret(interpreter);

    // We are done with the body now so we can set the function system back to normal.
    interpreter->setFunctionSystem(old_function_sys);
    // Let's also reset the current scope back to the old one
    interpreter->setCurrentScope(old_scope);

    /**
     * If the class extends Permission then we should add the permission to the current interpreters scope permissions
     * as this means we have just created a permission so we should get rights to it*/
    if (c->instanceOf("Permission"))
    {
        // We need to set the permissions of this new PermissionObject to that of the current interpreter scope
        std::shared_ptr<PermissionObject> perm_obj = std::dynamic_pointer_cast<PermissionObject>(Object::create(c));
        perm_obj->permissions = interpreter->getCurrentScope()->permissions;
        interpreter->getCurrentScope()->permissions->addPermission(perm_obj);
    }

    Value v;
    return v;
}

void ClassNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Class nodes do not support evaluation");
}
