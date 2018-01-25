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
    * All classes that should be validated should create a new object instance for that class
    * and call this method. It is a requirement as the validator needs to maintain scopes which can only be done with an Object instance.
    */
    void giveClassObject(std::shared_ptr<Object> object);
    bool isInClass();
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
    Class* current_class;
};

#endif
