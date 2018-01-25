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
#include "exceptionobject.h"
#include "exceptions/IOException.h"
#include "exceptions/systemexception.h"
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


Interpreter::Interpreter(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem) : SystemHandler(SYSTEM_HANDLER_INTERPRETER, classSystem, baseFunctionSystem, SYSTEM_HANDLER_NO_PARENT_BASE_CLASS_LINK)
{   
    this->output = [](const char* data)
    {
        std::cout << data;
    };
  
    this->input = []() -> std::string {
        std::string s;
        std::cin >> s;
        return s;
    };
    
    if (getClassSystem()->hasClassWithName("Object")) {
        throw std::logic_error("The Interpreter will create a class of Object. However the class system provided to it already has a class named Object. Please rename this class and rebuild.");
    }

    Class* obj_class = Interpreter::registerDefaultObjectClass(getClassSystem(), "Object");
    getClassSystem()->setDefaultBaseClass(obj_class);
   
    Class* c = getClassSystem()->registerClass("array");
    c->registerFunction("size", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = array->count;
    });

    /* Let's register an Exception class that is to be inherited by all classes that can be thrown*/
    Class* exception_class = getClassSystem()->registerClass("Exception");
    exception_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        
    });
    exception_class->registerFunction("getStackTrace", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        std::shared_ptr<ExceptionObject> exception_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = exception_obj->getStackTrace();
    });
    // The Exception class has the Exception object instance.
    exception_class->setDescriptorObject(std::make_shared<ExceptionObject>(exception_class));
    
    c = getClassSystem()->registerClass("NullPointerException", exception_class);
        c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
    });
   
    c = getClassSystem()->registerClass("IOException", exception_class);
        c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
    });
   this->lastFunctionCallNode = NULL;
   this->moduleSystem = NULL;
   this->first_run = true;

}

Interpreter::~Interpreter()
{

}

void Interpreter::setOutputFunction(OUTPUT_FUNCTION output)
{
    this->output = output;
}

void Interpreter::setInputFunction(INPUT_FUNCTION input)
{
    this->input = input;
}

void Interpreter::setModuleSystem(ModuleSystem* moduleSystem)
{
    if (this->moduleSystem != NULL)
        throw std::logic_error("This Interpreter is already bound to a ModuleSystem");
    this->moduleSystem = moduleSystem;
}

void Interpreter::ready()
{
    if (output == NULL)
    {
        throw std::logic_error("Expecting an output function before running a script. Use setOutputFunction(OUTPUT_FUNCTION outputFunction)");
    }

    if (this->moduleSystem == NULL)
        throw std::logic_error("The module system must be set. Use setModuleSystem");
}

void Interpreter::addToStackTrace(std::string function_name, PosInfo posInfo)
{
    struct stack_log_part part;
    part.function_name = function_name;
    part.posInfo = posInfo;
    this->stack_log.push_back(part);    
}

void Interpreter::popFromStackTrace()
{
    this->stack_log.pop_back();
}

std::vector<struct stack_log_part> Interpreter::getStackTraceLog()
{
    return this->stack_log;
}

void Interpreter::setCurrentBreakable(Breakable* breakable)
{
    this->breakables.push_back(breakable);
}

bool Interpreter::hasBreakable()
{
    return this->breakables.size() != 0;
}

    
Breakable* Interpreter::getCurrentBreakable()
{
    return this->breakables.back();
}

void Interpreter::finishBreakable()
{
    this->breakables.pop_back();
}

void Interpreter::run(const char* code, PosInfo posInfo)
{
    if (this->first_run) {
        // Let's tell the Modules about us!
        this->moduleSystem->tellModules(this);
        this->first_run = false;
    }

    ready();
    if (lexer == NULL)
        lexer = std::unique_ptr<Lexer>(new Lexer(&logger, posInfo));
    lexer->setInput(code, strlen(code));
    Token* root_token = lexer->lex();
    Token* token = root_token;
    while(token != NULL)
    {
        std::cout << token->getType() << ": " << token->getValue() << " :  line no: " << token->posInfo.line << ", col: " << token->posInfo.col << std::endl;
        token = token->next;
    }
    if (parser == NULL)
        parser = std::unique_ptr<Parser>(new Parser(&logger));
    Node* root_node;

    root_node = parser->parse(root_token);
    if (validator == NULL)
    {
        validator = std::unique_ptr<Validator>(new Validator(&logger, this));
        // We must set the validators previous scope to our own so that native variables are recognised.
        validator->getCurrentScope()->prev = this->getCurrentScope();
    }

    InterpretableNode* current_node = (InterpretableNode*) root_node;
    try
    {
        validator->validate(root_node);
        // Awesome now lets interpret!
        while(current_node != NULL)
        {
            current_node->interpret(this);
            current_node = (InterpretableNode*) current_node->next;
        }
    } 
    catch(SystemException& ex)
    {
        logger.error("System threw a " + ex.getObject()->getClass()->name + " exception" , current_node->posInfo);
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


void Interpreter::setLastFunctionCallNode(FunctionCallNode* fc_node)
{
    this->lastFunctionCallNode = fc_node;
}

FunctionCallNode* Interpreter::getLastFunctionCallNode()
{
    return this->lastFunctionCallNode;
}



Class* Interpreter::registerDefaultObjectClass(ClassSystem* class_system, std::string class_name)
{
    Class* c = class_system->registerClass(class_name, NULL, CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER);
    return c;
}