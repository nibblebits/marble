#ifndef CLASSSYSTEM_H
#define CLASSSYSTEM_H
#include <vector>
#include <memory>
#include "class.h"

class SystemHandler;
/** 
 * The ClassSystem is in charge of maintaining classes this includes registering and retrieving classes.
 * The ClassSystem can also have previous class systems. This is required so when a new SystemHandler is created all previously registered classes
 * do not have to be lost. You just pass the class system in the constructor of the SystemHandler. 
 * When retrieving a class from the ClassSystem with the getClassByName() method if the class is not found the ClassSystem will check the previous
 * class systems if it has any.
 * \attention Each ClassSystem is independant from any others. By setting a previous class system it does not mean that both these ClassSystems will share properties they will not. This means that if one ClassSystem has a default base class of Object and another has a default base class of Foo both these base classes will remain the same upon setting a previous class system. This also means that the SystemHandler will remain the same for both ClassSystems.
 **/
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
    std::vector<Class*> getAllClasses();
    std::vector<std::unique_ptr<Class>> classes;
private:
    ClassSystem* prev_sys;
    SystemHandler* sys_handler;
    Class* defaultBaseClass;
};

#endif
