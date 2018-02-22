#include "writtenfunction.h"
#include "interpreter.h"
#include "fnode.h"
#include "varnode.h"
#include "bodynode.h"
WrittenFunction::WrittenFunction(FunctionNode* function_node, std::vector<VarType> argument_types, VarType return_type) : SingleFunction(FUNCTION_TYPE_WRITTEN, function_node->name, argument_types, return_type)
{
    this->fnode = function_node;
    this->return_node = NULL;
}
WrittenFunction::~WrittenFunction()
{

}

bool WrittenFunction::hasReturned()
{
    return this->return_node != NULL;
}

FunctionNode* WrittenFunction::getFunctionNode()
{
    return this->fnode;
}


void WrittenFunction::invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    Function::invoke(interpreter, values, return_value, object);
    if (this->return_node != NULL)
    {
        return_value->set(&this->return_value);
    }
    this->return_node = NULL;
}

void WrittenFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Function arguments require there own scope
    interpreter->new_parented_scope();
    int count = 0;
    for (VarNode* arg : this->fnode->args)
    {
        // Interpret the variable node, this will end up creating a variable and adding it to our new scope
        Value var_value = arg->interpret(interpreter);
        // Now we must apply the value given for this argument
        if (count < values.size())
        {
            Variable* var = var_value.holder;
            var->value = values[count];
            // The new values holder must be to the variable we just created.
            var->value.holder = var;
        }
        count++;
    }
   
    std::cout << "WRITTEN FUNCTION INVOKED: " + this->name << std::endl;
    // Bodys also need a scope of there own
    interpreter->new_parented_scope();

    this->fnode->body->interpret(interpreter);
    // Finish the function body scope
    interpreter->finish_parented_scope();
    // Finish the function arguments scope
    interpreter->finish_parented_scope();
   
}
