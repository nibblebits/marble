#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <semaphore.h>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "systemhandler.h"
#include "modulesystem.h"
#include "logger.h"
#include "posinfo.h"
#include "breakable.h"
#include "splitter.h"
#include "statics.h"

typedef std::function<void(const char* output, int length)> OUTPUT_FUNCTION;
typedef std::function<std::string()> INPUT_FUNCTION;

struct stack_log_part
{
    std::string function_name;
    PosInfo posInfo;
};


class Parser;
class Lexer;
class Validator;

class Node;
class VarNode;
class ExpNode;
class BodyNode;
class Object;
class FunctionCallNode;
class Class;
class ClassSystem;
class CommonModule_SqlDriver;

class Interpreter : public SystemHandler
{
public:
    Interpreter(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem);
    virtual ~Interpreter();
    static Class* getDefaultBaseClass();

    /**
     * Sets the timeout for this Interpreter. Scripts will stop running if the run time goes higher
     * than the timeout provided. Use 0 if you do not want a timeout.
     * 
     * \attention The script is not guaranteed to stop straight away if the run time proceeds the timeout. This is because native functions are not bound to this timeout limit
     * \param seconds The time in seconds you want this script to timeout if it runs longer than the timeout provided
     */
    void setTimeout(int seconds);

    /**
     * Returns the current timeout for this interpreter
     */
    int getTimeout();
    
    /**
     * Call this function if you want the Interpreter to check if it has timed out
     * if this is true a TimeoutException is thrown
     * \throws TimeoutException
     */
    void checkTimeout();

    void setOutputFunction(OUTPUT_FUNCTION output);
    void setInputFunction(INPUT_FUNCTION input);
    /**
     * Sets the ModuleSystem for this interpreter. You should load your modules before calling this method to ensure modules are told about this interpreter.
     * Upon calling this method special module functions are also created allowing for loading of modules from within the interpreter its self.
     * The interpreter will tell the modules it loads about it's self.
     * IO_ERROR_H
     * \attention The module functions created must not be used when the ModuleSystem is no longer being used in a single threaded environment. As soon as the ModuleSystem is being used between threads these functions should not be allowed so do not provide the permission to the interpreter for the functions to be called.
     */
    void setModuleSystem(ModuleSystem* moduleSystem);
    void ready();

    Node* getAST(const char* code, PosInfo posInfo);
    void run(const char* code, PosInfo posInfo, bool ignore_validation=false);
    void runScript(const char* filename);
    Splitter loadScript(const char* filename);
    
    std::string handleSplitData(data_descriptor* descriptor);
    std::string handleCodeDataForSplit(split* split);
    std::string handleRawDataForSplit(split* split);
    std::string mergeCodeAndDataForSplit(split* split);
    void handleSplitterSplits(Splitter& splitter, PosInfo& posInfo);
    void handleLineAndColumn(PosInfo& posInfo, const char* data, int length);

    /**
     * Registers the default object class in the class_system provided and returns the Class.
     * \note This does not set the default object descriptor in the class_system you must do this manually.
     */
    static Class* registerDefaultObjectClass(ClassSystem* class_system, std::string class_name);

    /**
    * Should be used to set the last FunctionCallNode that was interpreted
    */
    void setLastFunctionCallNode(FunctionCallNode* fc_node);
    /**
    * Returns the last FunctionCallNode that was interpreted
    */
    FunctionCallNode* getLastFunctionCallNode();
    void addToStackTrace(std::string function_name, PosInfo posInfo);
    void popFromStackTrace();
    std::vector<struct stack_log_part> getStackTraceLog();
    
    /**
    * Sets the current Breakable entity. This could be a while loop, for loop or something else.
    * The Breakable entity that gets set is the entity that is currently being processed or dealt with such as interpreting the body of a while loop.
    */
    void setCurrentBreakable(Breakable* breakable); 
    
    /**
    * Returns true if there is a Breakable available. Breakables are set using the setCurrentBreakable method.
    */
    bool hasBreakable();
    
    /**
    *  Gets the current Breakable entity such as a while or for loop that is currently being interpreted
    */
    Breakable* getCurrentBreakable();
    
    /**
    * Finishes the current Breakable and must be called at some point after the setCurrentBreakable method is called.
    */
    void finishBreakable();
    
    /**
     * Returns true if this Interpreter instance has run the script with the address provided.
     * \param script_address A local URI to the script that you wish to check has been run.
     */
    bool hasRunScript(std::string script_address);
    /**
     * Returns true if this Interpreter instance has not finished running the script with the address provided.
     * \param script_address A local URI to the script that you wish to check is currently running
     */
    bool isNestedScript(std::string script_address);

    /**
     * If set to true then this interpreter will not be restricted by the permission system in anyway
     * and will be able to do anything with or without the permission to do so.
     */
    void setNoPermissionRestrictions(bool allow);

    /**
     * Returns true if this interpreter is not restricted to permissions.
     * If this is true this interpreter will not be bound to the permission system and can do anything.
     * 
     * \attention If you create a native library it is important that you check this if you are restricting programmers based on permissions. If this is true do not restrict them.
     */
    bool hasNoPermissionRestrictions();

    /**
     * Registers a SQL driver into the system.
     * \attention The driver provided must be a unique object for each interpreter instance
     */

    void registerSQLDriver(std::shared_ptr<CommonModule_SqlDriver> sql_driver);

    /**
     * Returns a registered SQL driver from the system
     */
    std::shared_ptr<CommonModule_SqlDriver> getSQLDriver(std::string driver_name);

    /**
     * Calls the output function assigned to this Interpreter
     * 
     * This is used when you need to provide output to the user through the console or to send data to the users web browser or something else regarding sending output.
     * Depending on the output functin will depend how the output is sent but the end goal of this function is to provide output to the user.
     */
    OUTPUT_FUNCTION output;

    /**
    * Calls the input function assinged to this Interpreter
    * The input function should return a string based on the input of the interpreter.
    * Such a string from std::cin to receive a string from the terminal.
    */
    INPUT_FUNCTION input;

private:
    /**
     * Creates the default classes and functions that the marble interpreter requires to function properly
     * all other resources that do not affect weather the marble interpreter runs correctly or not should not be loaded in this function
     */
    void createDefaultClassesAndFunctions();
    void setupValidator();
    void setupModuleMarbleFunctions(ModuleSystem* moduleSystem);
    void fail();
    const char* filename;
    bool first_run;
    // Set to true if permission checks should not take place.
    bool no_permission_restritions;
    ModuleSystem* moduleSystem;
    FunctionCallNode* lastFunctionCallNode;
    std::vector<std::string> nested_scripts_run;
    std::vector<std::string> scripts_run;
    std::vector<struct stack_log_part> stack_log;
    std::vector<Breakable*> breakables;
    // The registered sql drivers for this interpreter instance
    std::vector<std::shared_ptr<CommonModule_SqlDriver>> sql_drivers;
    std::unique_ptr<Lexer> lexer;
    std::unique_ptr<Parser> parser;
    std::unique_ptr<Validator> validator;

    // This is the timeout for this Interpreter instance. Scripts will stop running if this is hit
    int timeout;

    // This is equal to the time the first run was called
    time_t execution_started;

};

#endif
