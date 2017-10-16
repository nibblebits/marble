#ifndef CLASSSYSTEM_H
#define CLASSSYSTEM_H
#include <vector>
#include <memory>
#include "class.h"

class SystemHandler;
class ClassSystem
{
public:
    ClassSystem();
    virtual ~ClassSystem();
    void setSystemHandler(SystemHandler* sys_handler);
    void setPreviousClassSystem(ClassSystem* prev_sys);
    ClassSystem* getPreviousClassSystem();
    void setDefaultBaseClass(Class* c);
    Class* getDefaultBaseClass();
    Class* registerClass(std::string class_name, Class* parent=NULL);
    Class* getClassByName(std::string name);
    bool hasClassWithName(std::string name);
    std::vector<std::unique_ptr<Class>> classes;
private:
    ClassSystem* prev_sys;
    SystemHandler* sys_handler;
    Class* defaultBaseClass;
};

#endif
