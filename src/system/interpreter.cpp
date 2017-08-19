/*
 No validation is done in this interpreter class as the interpreter will pass the AST(Abstract Syntax Tree) through a semantic validator to ensure validity.
 In other words its completely safe not to validate in this file.
*/
#include <iostream>
#include <memory.h>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include "interpreter.h"
#include "splitter.h"
#include "lexer.h"
#include "parser.h"
#include "nodes.h"
#include "debug.h"
#include "scope.h"
#include "variable.h"
#include "exceptions/IOException.h"
Interpreter::Interpreter()
{
    this->current_scope = &root_scope;

    getFunctionSystem()->registerFunction("print", [&](std::vector<Value> arguments, Value* return_value) {
        for (Value v : arguments)
        {
            if (v.type == VALUE_TYPE_NUMBER)
            {
                std::cout << "OUTPUT: " << v.dvalue << std::endl;
            }
            else if(v.type == VALUE_TYPE_STRING)
            {
               std::cout << "OUTPUT: " << v.svalue << std::endl;
            }
        }
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = 1;
    });
}

Interpreter::~Interpreter()
{

}

void Interpreter::output(const char* data)
{
    /* Here a special output function would be called
     * instead of direct I/O. This would allow the user of the Marble library to determine how output should be handled. For example if this is running on a standalone application it should output to the console, or if its a web application it should output to the web client of the browsing user.*/
    std::cout << data << std::endl;
}

void Interpreter::run(const char* code)
{
    Lexer lexer;
    lexer.setInput(code, strlen(code));
    Token* root_token = lexer.lex();
    Token* token = root_token;
    while(token != NULL)
    {
        std::cout << token->getType() << ": " << token->getValue() << std::endl;
        token = token->next;
    }

    Parser parser;
    Node* root_node = parser.parse(root_token);
    Node* current_node = root_node;

    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        int type = current_node->getType();
        switch (type)
        {
            case NODE_TYPE_VARIABLE_DECLARATION:
            {
                interpret_variable_node((VarNode*) current_node);
            }
            break;
            case NODE_TYPE_EXPRESSION:
            {
                ExpNode* exp_node = (ExpNode*)  current_node;
                exp_node->interpret(this);
            }
            break;

            case NODE_TYPE_FUNCTION_CALL:
            {
                FunctionCallNode* func_call_node = (FunctionCallNode*) current_node;
                func_call_node->interpret(this);
            }
            break;
        }

        current_node = current_node->next;
    }

    for (Variable* variable : current_scope->getVariables())
    {
        std::cout << "Variable: " << variable->name << " = " << variable->value.dvalue << std::endl;
    }
}


void Interpreter::fail()
{
    throw std::logic_error("Something has gone terribly wrong, semantic validation has clearly messed up. Please report this");
}


FunctionSystem* Interpreter::getFunctionSystem()
{
    return &this->functionSystem;
}


Variable* Interpreter::getVariableByName(std::string name)
{
    // Let's not worry about other nested scopes at the moment. We will just check the current scope and implement the rest another time. Don't run before you can walk.
    Variable* variable = current_scope->getVariable(name);
    return variable;
}

void Interpreter::interpret_variable_node_for_primitive(VarNode* var_node)
{
    Node* type_node = var_node->type;
    std::string name = var_node->name;
    ExpressionInterpretableNode* value_node = (ExpressionInterpretableNode*)var_node->value;

    Variable* variable = current_scope->createVariable();
    KeywordNode* type_node_keyword = (KeywordNode*) type_node;
    variable->value = value_node->interpret(this);
    variable->value.holder = variable;
    variable->name = name;
    current_scope->registerVariable(variable);
}

void Interpreter::interpret_variable_node(VarNode* var_node)
{
    Node* type_node = var_node->type;
    switch (type_node->type)
    {
        case NODE_TYPE_KEYWORD:
        {
            interpret_variable_node_for_primitive(var_node);
        }
        break;
    }
}


double Interpreter::op_on_values(double value1, double value2, std::string op)
{
    if (op == "+")
    {
        return value1 + value2;
    }
    else if(op == "-")
    {
        return value1 - value2;
    }
    else if(op == "*")
    {
        return value1 * value2;
    }
    else if(op == "/")
    {
        return value1 / value2;
    }

    else
    {
        throw std::logic_error("Unexpected operator: " + op);
    }
}
void Interpreter::runScript(const char* filename)
{
    // Lets load this script
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        throw IOException("Failed to open file: " + std::string(filename));
    }

    if(fseek(file, 0, SEEK_END) != 0)
    {
        throw IOException("Failed to seek to the end of the file: " + std::string(filename));
    }

    long data_len = ftell(file);
    rewind(file);
    char* data = new char[data_len];
    fread(data, data_len, 1, file);

    Splitter splitter;
    splitter.setData(data, data_len);

    split split;
    while(splitter.split(&split))
    {
        // Output the data
        if (split.has_data)
        {
            char* output_data = new char[split.output.size+1];
            memcpy(output_data, split.output.data, split.output.size);
            output_data[split.output.size] = 0;
            output(output_data);
            delete output_data;
        }

        // Run the code
        if (split.has_code)
        {
            char* code_data = new char[split.code.size+1];
            memcpy(code_data, split.code.data, split.code.size);
            code_data[split.code.size] = 0;
            run(code_data);
            delete code_data;
        }
    }

    // Close and clean up
    fclose(file);
    delete data;
}

