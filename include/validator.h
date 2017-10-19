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
class Validator : public SystemHandler
{
public:
    Validator(Logger* logger, ClassSystem* classSystem, FunctionSystem* baseFunctionSystem);
    virtual ~Validator();
    void validate(Node* root_node);
    void giveClassObject(std::shared_ptr<Object> object);
    bool isInClass();
    Object* getClassObject(std::string name);
    void beginClass(Class* current_class);
    Class* getCurrentClass();
    void endClass();
    void save();
    void restore();
    void expecting(VALUE_TYPE type);
    void expectingObject(std::string obj_name);
    void expectingArray(int dimensions);
    bool isExpecting();
    bool isExpectingArray();
    int getExpectedArrayDimensions();
    void endExpecting();
    VALUE_TYPE getExpectingType();
    std::string getExpectingObject();
private:
    Logger* logger;
    
    // We must be able to stack the rules so that they can be reset for different operations
    struct rules
    {
        VALUE_TYPE expecting_type = -1;
        std::string expecting_object = "";
        // Set to zero if we are not expecting an array
        int expected_array_dimensions = 0;
    };
    
    struct rules rules;
    std::vector<struct rules> rules_stack;
    std::vector<std::shared_ptr<Object>> class_objects;
    Class* current_class;
};

#endif
