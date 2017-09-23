#ifndef CLASSSYSTEM_H
#define CLASSSYSTEM_H
#include <vector>
#include <memory>
#include "class.h"

class Interpreter;
class ClassSystem
{
public:
    ClassSystem();
    virtual ~ClassSystem();
    void setInterpreter(Interpreter* interpreter);
    Class* registerClass(std::string class_name);
    Class* getClassByName(std::string name);
    std::vector<std::unique_ptr<Class>> classes;
private:
    Interpreter* interpreter;
};

#endif
