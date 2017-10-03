#include "classnode.h"
#include "csystem.h"
#include "nodes.h"
#include "interpreter.h"
#include "scope.h"

ClassNode::ClassNode() : InterpretableNode(NODE_TYPE_CLASS)
{
    this->body = NULL;
}

ClassNode::~ClassNode()
{

}

Value ClassNode::interpret(Interpreter* interpreter)
{
    ClassSystem* c_system = interpreter->getClassSystem();
    Class* c = c_system->registerClass(name, NULL);
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

