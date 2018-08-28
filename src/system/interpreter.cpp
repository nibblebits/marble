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
#include <algorithm>
#include <errno.h>

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
#include "misc.h"
#include "module.h"
#include "exceptionobject.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "filepermission.h"
#include "modulehandlingpermissionobject.h"
#include "commonmod_sqldriver.h"
#include "exceptions/IOException.h"
#include "exceptions/systemexception.h"
#include "exceptions/timeoutexception.h"

std::string getAllVariablesAsString(Scope *scope)
{
    std::string return_val = "";
    for (Variable *var : scope->getVariables())
    {
        return_val += var->name + ", ";
    }

    if (scope->prev != NULL)
        return_val += getAllVariablesAsString(scope->prev);

    return return_val;
}

Interpreter::Interpreter(ClassSystem *classSystem, FunctionSystem *baseFunctionSystem) : SystemHandler(SYSTEM_HANDLER_INTERPRETER, classSystem, baseFunctionSystem, SYSTEM_HANDLER_NO_PARENT_BASE_CLASS_LINK)
{
    this->output = [](const char *data, int length) {
        for (int i = 0; i < length; i++)
        {
            std::cout << (char)data[i];
        }
    };

    this->input = []() -> std::string {
        std::string s;
        std::cin >> s;
        return s;
    };

    // Creat ethe default classes and functions for this interpreter
    createDefaultClassesAndFunctions();

    // We must now create a permission object for our global scope
    getGlobalScope()->permissions = std::dynamic_pointer_cast<PermissionsObject>(Object::create(getClassSystem()->getClassByName("Permissions")));

    this->lastFunctionCallNode = NULL;
    this->moduleSystem = NULL;
    this->first_run = true;
    this->no_permission_restritions = false;
    this->timeout = 0;
    this->execution_started = 0;
}

Interpreter::~Interpreter()
{
    // Join the threads before we leave
    for (auto &t : this->active_threads)
        t.join();
}

void Interpreter::addThread(std::thread t)
{
    this->active_threads.push_back(std::move(t));
}

void Interpreter::setTimeout(int seconds)
{
    this->timeout = seconds;
}

int Interpreter::getTimeout()
{
    return this->timeout;
}

void Interpreter::checkTimeout()
{
    // No timeout
    if (this->timeout == 0)
        return;

    if (time(NULL) - this->execution_started > this->timeout)
        throw TimeoutException("The interpreter timed out");
}

void Interpreter::setOutputFunction(OUTPUT_FUNCTION output)
{
    if (this->output != NULL)
    {
        // We must save the current output function
        this->output_function_stack.push_back(this->output);
    }
    this->output = output;
}

void Interpreter::setInputFunction(INPUT_FUNCTION input)
{
    this->input = input;
}

void Interpreter::finishOutputFunction()
{
    if (!this->output_function_stack.empty())
    {
        this->output = this->output_function_stack.back();
        this->output_function_stack.pop_back();
    }
}

void Interpreter::setModuleSystem(ModuleSystem *moduleSystem)
{
    if (this->moduleSystem != NULL)
        throw std::logic_error("This Interpreter is already bound to a ModuleSystem");
    this->moduleSystem = moduleSystem;

    // Let's create some module functions for modules to be loaded from within marble language
    setupModuleMarbleFunctions(moduleSystem);

    // Let's tell the modules about ourself.
    this->moduleSystem->tellModules(this);
}

void Interpreter::setupModuleMarbleFunctions(ModuleSystem *moduleSystem)
{
    /* 
    * This function allows people to load marble modules from within the marble language
    * The function accepts one argument of type string and this should equal the filename you wish to load
    * 
    * Prototype:
    * function LoadModule(string filename) : void
    * throws PermissionException if interpreter does not have the correct permissions
    * throws IOException if the module fails to load
    */
    getFunctionSystem()->registerFunction("LoadModule", {VarType::fromString("string")}, VarType::fromString("void"), [=](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<ModuleHandlingPermissionObject> permission = std::dynamic_pointer_cast<ModuleHandlingPermissionObject>(caller_scope->permissions->getPermission("ModuleHandlingPermission"));
        if (!interpreter->hasNoPermissionRestrictions())
        {
            if (permission == NULL)
            {
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))));
            }
        }
        std::string filename = arguments[0].svalue;
        try
        {
            Module *module = moduleSystem->loadModule(filename.c_str());
            // Tell the module about us.
            module->newInterpreter(this);
        }
        catch (...)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(getClassSystem()->getClassByName("IOException"))));
        }
    });
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

bool Interpreter::hasRunScript(std::string script_address)
{
    std::string abs_address = getAbsolutePath(script_address);
    for (std::string val : this->scripts_run)
    {
        if (val == abs_address)
            return true;
    }

    return false;
}

bool Interpreter::isNestedScript(std::string script_address)
{
    std::string abs_address = getAbsolutePath(script_address);
    for (std::string val : this->nested_scripts_run)
    {
        if (val == abs_address)
            return true;
    }

    return false;
}

Node *Interpreter::getAST(const char *code, PosInfo posInfo)
{
    ready();
    if (lexer == NULL)
        lexer = std::unique_ptr<Lexer>(new Lexer(&logger));
    lexer->setInput(code, strlen(code));
    Token *root_token = lexer->lex(posInfo);
    Token *token = root_token;
    while (token != NULL)
    {
        token = token->next;
    }

    if (parser == NULL)
        parser = std::unique_ptr<Parser>(new Parser(&logger));
    Node *root_node;

    root_node = parser->parse(root_token);
    return root_node;
}

void Interpreter::createDefaultClassesAndFunctions()
{
    Class *c = getClassSystem()->registerClass("array");
    c->registerFunction("size", {}, VarType::fromString("number"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = array->count;
    });

    Class *exception_class = getClassSystem()->getClassByName("Exception");
    if (exception_class == NULL)
        throw std::logic_error("The Interpreter expects a class with the name Exception to exist please create this in parent class system");

    if (std::dynamic_pointer_cast<ExceptionObject>(exception_class->getDescriptorObject()) == NULL)
        throw std::logic_error("The Exception class registered in a parent class system has a descriptor object that does not extend the ExceptionObject native class");

    c = getClassSystem()->registerClass("OutOfBoundsException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("LogicException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("InvalidIndexException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("InvalidTypeException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("NullPointerException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("InvalidCastException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("IOException", exception_class);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("InfiniteLoopException", exception_class);
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("InvalidEntityException", exception_class);
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("VariableLockedException", exception_class);
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
    });

    c = getClassSystem()->registerClass("EntityNotRegisteredException", exception_class);
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        Function *parent_constructor = object->getClass()->parent->getFunctionByNameAndArguments("__construct", {VarType::fromString("string")});
        parent_constructor->invoke(interpreter, arguments, return_value, object, caller_scope);
    });

    // We must now create a permission object for our root scope
    getGlobalScope()->permissions = std::dynamic_pointer_cast<PermissionsObject>(Object::create(getClassSystem()->getClassByName("Permissions")));

    // It is also wise to create a method for getting the current permissions
    getFunctionSystem()->registerFunction("getScopePermissions", {}, VarType::fromString("Permissions"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = interpreter->getCurrentScope()->permissions;
    });

    getFunctionSystem()->registerFunction("getCallerPermissions", {}, VarType::fromString("Permissions"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        return_value->type = VALUE_TYPE_OBJECT;
        // The _caller_permissions variable is a special variable registered when calling a function. It only exists for the scope of the function
        // You can find this at function.cpp
        return_value->ovalue = interpreter->getCurrentScope()->getVariableAnyScope("_caller_permissions")->value.ovalue;
    });
}

void Interpreter::setupValidator()
{
    if (validator == NULL)
    {
        validator = std::unique_ptr<Validator>(new Validator(&logger, this));
    }

    // We must set the validators previous scope to our own so that native variables are recognised.
    validator->getRootScope()->prev = this->getCurrentScope();
}

Validator *Interpreter::getValidator()
{
    return this->validator.get();
}

void Interpreter::run(const char *code, PosInfo posInfo, bool ignore_validation)
{
    bool this_run_started_execution = false;
    if (this->execution_started == 0)
    {
        // Execution has just started
        this_run_started_execution = true;
        this->execution_started = time(NULL);
    }

    ready();
    setupValidator();
    Node *root_node = getAST(code, posInfo);
    InterpretableNode *current_node = (InterpretableNode *)root_node;
    try
    {
        if (!ignore_validation)
            validator->validate(root_node);
        // Awesome now lets interpret!
        while (current_node != NULL)
        {
            Debug::PrintValueForNode(current_node);
            current_node->interpret(this);
            current_node = (InterpretableNode *)current_node->next;
        }
    }
    catch (SystemException &ex)
    {
        std::shared_ptr<ExceptionObject> rex = std::dynamic_pointer_cast<ExceptionObject>(ex.getObject());
        logger.error("System threw a " + rex->getClass()->name + ", message: " + rex->getMessage(), current_node->posInfo);
    }

    if (this_run_started_execution)
    {
        // We are done reset the execution time
        this->execution_started = 0;
    }
}

void Interpreter::fail()
{
    throw std::logic_error("Something has gone terribly wrong, semantic validation has clearly messed up. Please report this");
}

void Interpreter::handleLineAndColumn(PosInfo &posInfo, const char *data, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (data[i] == 0x0a)
        {
            posInfo.line += 1;
            posInfo.col = 1;
        }
        else
        {
            posInfo.col += 1;
        }
    }
}

std::string Interpreter::handleSplitData(data_descriptor *descriptor)
{
    std::string data = "";
    CloneForCall(descriptor->data, descriptor->size, descriptor->size + 1, [&](const void *ptr, int size) {
        char *raw_data = (char *)ptr;
        raw_data[descriptor->size] = 0;
        data = raw_data;
    });

    return data;
}

std::string Interpreter::handleCodeDataForSplit(split *split)
{
    return handleSplitData(&split->code);
}

std::string Interpreter::handleRawDataForSplit(split *split)
{
    return handleSplitData(&split->output);
}

std::string Interpreter::mergeCodeAndDataForSplit(split *split)
{
    // This is any data parsed
    std::string output_data = "";
    std::string code_data = "";
    std::string result = "";

    // Output the data
    if (split->has_data)
    {
        output_data = handleRawDataForSplit(split);
        result += "\"" + str_replace(output_data, "\"", "\\\"") + "\";\n";
    }

    // Run the code
    if (split->has_code)
    {
        code_data = handleCodeDataForSplit(split);
        result += code_data;
    }

    return result;
}

void Interpreter::handleSplitterSplits(Splitter &splitter, PosInfo &posInfo)
{
    split split;
    std::string result = "";

    while (splitter.split(&split))
    {
        result += mergeCodeAndDataForSplit(&split);
    }

    // Finally run the merged code
    run(result.c_str(), posInfo);
}

void Interpreter::runScript(const char *filename)
{
    // Setup positioning information
    PosInfo posInfo;
    posInfo.filename = filename;
    posInfo.line = 1;
    posInfo.col = 1;

    Splitter splitter = loadScript(filename);
    handleSplitterSplits(splitter, posInfo);

    // Free the splitter data
    splitter.free();
    this->filename = this->nested_scripts_run.back().c_str();
    this->nested_scripts_run.pop_back();
}

Splitter Interpreter::loadScript(const char *filename)
{
    if (this->isNestedScript(std::string(filename)))
    {
        //throw IOException("The script: " + std::string(filename) + " has already run once before in this run session. This script cannot run again as its possible this can result in an infinite loop");
    }
    this->nested_scripts_run.push_back(getAbsolutePath(filename));
    this->filename = filename;
    if (!hasRunScript(filename))
        this->scripts_run.push_back(getAbsolutePath(std::string(filename)));
    // Lets load this script
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        std::string open_error = strerror(errno);
        throw IOException("Failed to open file: " + std::string(filename) + ", " + open_error);
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        throw IOException("Failed to seek to the end of the file: " + std::string(filename));
    }

    long data_len = ftell(file);
    rewind(file);
    char *data = new char[data_len];
    fread(data, data_len, 1, file);

    Splitter splitter(&logger, filename);
    splitter.setData(data, data_len);

    // Close and clean up
    fclose(file);
    return splitter;
}

void Interpreter::setLastFunctionCallNode(FunctionCallNode *fc_node)
{
    this->lastFunctionCallNode = fc_node;
}

FunctionCallNode *Interpreter::getLastFunctionCallNode()
{
    return this->lastFunctionCallNode;
}

void Interpreter::setNoPermissionRestrictions(bool allow)
{
    this->no_permission_restritions = allow;
}

bool Interpreter::hasNoPermissionRestrictions()
{
    return this->no_permission_restritions;
}

void Interpreter::registerSQLDriver(std::shared_ptr<CommonModule_SqlDriver> sql_driver)
{
    // If we already have the driver just dont register it again
    if (getSQLDriver(sql_driver->getClass()->name))
        return;

    sql_drivers.push_back(sql_driver);
}

std::shared_ptr<CommonModule_SqlDriver> Interpreter::getSQLDriver(std::string driver_name)
{
    for (std::shared_ptr<CommonModule_SqlDriver> sql_driver : this->sql_drivers)
    {
        if (sql_driver->getClass()->name == driver_name)
            return sql_driver;
    }

    return NULL;
}

Class *Interpreter::registerDefaultObjectClass(ClassSystem *class_system, std::string class_name)
{
    Class *c = class_system->registerClass(class_name, NULL, CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER);
    c->registerFunction("toString", {}, VarType::fromString("string"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = "toString() for object not implemented";
    });
    return c;
}
