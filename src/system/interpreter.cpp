/*
 No validation is done in this interpreter class as the interpreter will pass the AST(Abstract Syntax Tree) through a semantic validator to ensure validity.
 In other words its completely safe not to validate in this file.
*/
#include <iostream>
#include <memory.h>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "interpreter.h"
#include "splitter.h"
#include "lexer.h"
#include "parser.h"
#include "nodes.h"
#include "debug.h"
#include "scope.h"
#include "variable.h"
#include "object.h"
#include "exceptions/IOException.h"
Interpreter::Interpreter()
{
    this->current_scope = &root_scope;
    this->output = [](const char* data)
    {
        std::cout << data;
    };

    getFunctionSystem()->registerFunction("print", [&](std::vector<Value> arguments, Value* return_value) {
        std::stringstream ss;
        for (Value v : arguments)
        {
            if (v.type == VALUE_TYPE_NUMBER)
            {
               ss << v.dvalue;
            }
            else if(v.type == VALUE_TYPE_STRING)
            {
               ss << v.svalue;
            }
            else
            {
               ss << "Invalid value type: " << v.type;
            } 
        }
        output(ss.str().c_str());
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = 1;
    });
    
     getFunctionSystem()->registerFunction("input_string", [&](std::vector<Value> arguments, Value* return_value) {
        return_value->type = VALUE_TYPE_STRING;
        std::cin >> return_value->svalue;
    });
    
}

Interpreter::~Interpreter()
{

}

void Interpreter::setOutputFunction(OUTPUT_FUNCTION output)
{
    this->output = output;
}

void Interpreter::interpret_body_node(Node* node)
{
    int type = node->getType();
    switch (type)
    {
        case NODE_TYPE_VARIABLE_DECLARATION:
        {
            interpret_variable_node((VarNode*) node);
        }
        break;
        case NODE_TYPE_EXPRESSION:
        {
            ExpNode* exp_node = (ExpNode*)  node;
            Debug::PrintValueForNode(exp_node);
	        exp_node->interpret(this);
        }
        break;

        case NODE_TYPE_FUNCTION_CALL:
        {
            FunctionCallNode* func_call_node = (FunctionCallNode*) node;
            func_call_node->interpret(this);
        }
        break;
        
        case NODE_TYPE_IF_STMT:
        {
            IfStatementNode* if_stmt_node = (IfStatementNode*) node;
            if_stmt_node->interpret(this);
        }
        break;
        default:
            throw std::logic_error("void Interpreter::interpret_body_node(Node* node): Unsure how to interpret node: " + std::to_string(type));
    }
}

void Interpreter::interpret_body(BodyNode* node)
{
    // Let's create a new parented scope for this
    new_parented_scope();
    
    Node* current_node = node->child;

    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        interpret_body_node(current_node);
        current_node = current_node->next;
    }
    
    // We are done with this cope
    finish_parented_scope();
   
}

void Interpreter::ready()
{
    if (output == NULL)
    {
        throw std::logic_error("Expecting an output function before running a script. Use setOutputFunction(OUTPUT_FUNCTION outputFunction)");
    }
}

void Interpreter::run(const char* code, PosInfo posInfo)
{
    ready();
    Lexer lexer(&logger, posInfo);
    lexer.setInput(code, strlen(code));
    Token* root_token = lexer.lex();
    Token* token = root_token;
    while(token != NULL)
    {
        std::cout << token->getType() << ": " << token->getValue() << " :  line no: " << token->posInfo.line << ", col: " << token->posInfo.col << std::endl;
        token = token->next;
    }

    Parser parser(&logger);
    Node* root_node;

    root_node = parser.parse(root_token);
    Node* current_node = root_node;
    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        interpret_body_node(current_node);
        current_node = current_node->next;
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

Scope* Interpreter::getCurrentScope()
{
    return this->current_scope;
}

Scope* Interpreter::getRootScope()
{
    return &this->root_scope;
}

Variable* Interpreter::getVariableByName(std::string name)
{
    Variable* variable = NULL;
    Scope* scope = current_scope;
    while(scope != NULL)
    {
        variable = scope->getVariable(name);
        if (variable != NULL)
        {
            break;
        }
        
        scope = scope->prev;
    }

    return variable;
}

void Interpreter::new_parented_scope()
{
    Scope* new_prev = current_scope;
    current_scope = new Scope();
    current_scope->prev = new_prev;
}

void Interpreter::finish_parented_scope()
{
    Scope* old_current = current_scope;
    current_scope = old_current->prev;
    delete old_current;
}

int Interpreter::getVariableTypeForString(std::string str)
{
    int type = VARIABLE_TYPE_OBJECT;
    if (str == "number")
    {
        type = VARIABLE_TYPE_NUMBER;
    }
    else if(str == "string")
    {
        type = VARIABLE_TYPE_STRING;
    }
    return type;
}
void Interpreter::interpret_variable_node_for_primitive(VarNode* var_node)
{
    int type = -1;
    KeywordNode* type_node = (KeywordNode*)var_node->type;
    
    std::string name = var_node->name;
    ExpressionInterpretableNode* value_node = (ExpressionInterpretableNode*)var_node->value;

    Variable* variable = current_scope->createVariable();
    KeywordNode* type_node_keyword = (KeywordNode*) type_node;
    Debug::PrintValueForNode(value_node);
    variable->value = value_node->interpret(this);
    variable->value.holder = variable;
    variable->name = name;
    variable->type = getVariableTypeForString(type_node->value);
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


void Interpreter::handleLineAndColumn(PosInfo* posInfo, const char* data, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (data[i] == 0x0a)
        {
            posInfo->line += 1;
            posInfo->col = 1;
        }
        else
        {
            posInfo->col+=1;
        }
    }
}

void Interpreter::runScript(const char* filename)
{
    this->filename = filename;
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
    
    PosInfo posInfo;
    posInfo.filename = filename;
    posInfo.line = 1;
    posInfo.col = 1;
    
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
            handleLineAndColumn(&posInfo, output_data, split.output.size);
            delete output_data;
        }

        // Run the code
        if (split.has_code)
        {
            char* code_data = new char[split.code.size+1];
            memcpy(code_data, split.code.data, split.code.size);
            code_data[split.code.size] = 0;
            // Marble tag "<marble>" should be added to the current position
            posInfo.col += strlen(MARBLE_OPEN_TAG);
            run(code_data, posInfo);
            handleLineAndColumn(&posInfo, code_data, split.code.size);
            delete code_data;
        }
    }

    // Close and clean up
    fclose(file);
    delete data;
}

Logger* Interpreter::getLogger()
{
    return &this->logger;
}

