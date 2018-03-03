#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <string>
#include <vector>
#include "scope.h"
#include "statics.h"
#include "systemhandler.h"
class Class;
class Logger;
class Node;
class Interpreter;
class Validator : public SystemHandler
{
public:
    Validator(Logger* logger, Interpreter* interpreter);
    virtual ~Validator();
    void validate(Node* root_node);
    
    Interpreter* getInterpreter();
    /**
     * Add's the provided object to this validators class object stack.
     * \param object The object to add to the class object stack
    */
    void giveClassObject(std::shared_ptr<Object> object);
    bool isInClass();
    /**
     * Gets an validation Object for the class with the name provided from the class object stack.
     * \param name The name of the class to get the validation Object for
     * \return Returns the validation Object for the class with the provided name
     */ 
    Object* getClassObject(std::string name);
    void beginClass(Class* current_class);
    Class* getCurrentClass();
    void endClass();
    void save();
    void restore();
    void expecting(std::string type);
    void expectingArray(int dimensions);
    bool isExpecting();
    bool isExpectingArray();
    int getExpectedArrayDimensions();
    void endExpecting();
    std::string getExpectingType();
    VALUE_TYPE getExpectingValueType();
    VARIABLE_TYPE getExpectingVariableType();

    /**
     *  Tells the validator to ignore validation when accessing an object with an instance of the class with the name provided.
     *  \param class_name The class name to ignore
     * 
     *  \note This ignore system exists because when you include a file you cannot guarantee at validation time that the class will exist
     *   as the file to include might only be known at runtime.
     */
    void ignoreClass(std::string class_name);
    /**
     *  Returns true if the class provided should be ignored for validation when accessing an instance of this class.
     *  \param class_name The class name to check is ignored
     */
    bool isClassIgnored(std::string class_name);
private:
    Interpreter* interpreter;
    Logger* logger;
    
    // We must be able to stack the rules so that they can be reset for different operations
    struct rules
    {
        VALUE_TYPE expecting_value_type = -1;
        VARIABLE_TYPE expecting_variable_type = -1;
        std::string expecting_type = "";
        // Set to zero if we are not expecting an array
        int expected_array_dimensions = 0;
    };
    
    struct rules rules;
    std::vector<struct rules> rules_stack;
    std::vector<std::shared_ptr<Object>> class_objects;

    // The ignored classes that validation should be ignored for as they are not declared classes.
    std::vector<std::string> ignored_classes;
    Class* current_class;
};

#endif
