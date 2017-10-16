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
#include "validator.h"
#include "nodes.h"
#include "debug.h"
#include "scope.h"
#include "variable.h"
#include "object.h"
#include "function.h"
#include "class.h"
#include "array.h"
#include "exceptions/IOException.h"
    std::string getAllVariablesAsString(Scope* scope)
    {
        std::string return_val = "";
        for (Variable* var : scope->getVariables())
        {
            return_val += var->name + ", ";
        }
        
        if (scope->prev != NULL)
            return_val += getAllVariablesAsString(scope->prev);
        
        return return_val;
    }
    
Interpreter::Interpreter(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem) : SystemHandler(SYSTEM_HANDLER_INTERPRETER, classSystem, baseFunctionSystem)
{   
    this->output = [](const char* data)
    {
        std::cout << data;
    };

    // Lets create an Object base class that will be the base class of all objects, we should also create an array class that will be used for arrays
    Class* c = getClassSystem()->registerClass("Object");
    c->registerFunction("toString", [&](std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = object->getClass()->name;
    });

    getClassSystem()->setDefaultBaseClass(c);
    
    c = getClassSystem()->registerClass("array");
    c->registerFunction("size",  [&](std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = array->count;
    });
    
    
    getBaseFunctionSystem()->registerFunction("print", [&](std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
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
    
    getBaseFunctionSystem()->registerFunction("input_string", [&](std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        return_value->type = VALUE_TYPE_STRING;
        std::cin >> return_value->svalue;
    });
    
    getBaseFunctionSystem()->registerFunction("variables", [&](std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = "Variables: " + getAllVariablesAsString(getCurrentScope());
    });
   
}

Interpreter::~Interpreter()
{

}

void Interpreter::setOutputFunction(OUTPUT_FUNCTION output)
{
    this->output = output;
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
    
    Validator validator(&logger, getClassSystem(), getBaseFunctionSystem());
    validator.validate(root_node);
    
    InterpretableNode* current_node = (InterpretableNode*) root_node;
    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        current_node->interpret(this);
        current_node = (InterpretableNode*) current_node->next;
    }

}


void Interpreter::fail()
{
    throw std::logic_error("Something has gone terribly wrong, semantic validation has clearly messed up. Please report this");
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

    Splitter splitter(&logger, filename);
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

