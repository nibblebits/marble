#include "writtenfunction.h"
#include "interpreter.h"
#include "fnode.h"
#include "varnode.h"
#include "bodynode.h"
WrittenFunction::WrittenFunction(Interpreter* interpreter, FunctionNode* function_node) : Function(function_node->name)
{
    this->interpreter = interpreter;
    this->fnode = function_node;
}
WrittenFunction::~WrittenFunction()
{

}
void WrittenFunction::invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Function arguments require there own scope
    interpreter->new_parented_scope();
    int count = 0;
    for (VarNode* arg : this->fnode->args)
    {
        // Interpret the variable node, this will end up creating a variable and adding it to our new scope
        Value var_value = arg->interpret(this->interpreter);
        // Now we must apply the value given for this argument
        if (count < values.size())
        {
            Variable* var = var_value.holder;
            var->value = values[count];
        }
        count++;
    }
   
    // Bodys also need a scope of there own
    interpreter->new_parented_scope();
    this->fnode->body->apply_node_listener([&](Node* node, Value v) -> bool {
        if (node->type == NODE_TYPE_RETURN)
        {
            // The body just saw a return node, lets return its value
            return_value->set(&v);
            return false;
        }
        
        return true;
    });
    this->fnode->body->interpret(this->interpreter);
    // Finish the function body scope
    interpreter->finish_parented_scope();
    // Finish the function arguments scope
    interpreter->finish_parented_scope();
   
}
