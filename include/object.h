#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <functional>
#include <vector>
#include "class.h"
#include "scope.h"
#include "value.h"
#include "statics.h"

class SystemHandler;
class Object : public Scope,  public std::enable_shared_from_this<Object>
{
public:
    Object(Class* c);
    Object(Class* c, std::shared_ptr<PermissionsObject> permissions);
    virtual ~Object();
    
    /**
    * Creates an Object instance for the given object class but does not invoke its constructor
    * This method is safe to call while not interpreting. If you are interpreting use the other "create" method that takes constructor values
    * so that the constructor is called and interpreted.
    */
    static std::shared_ptr<Object> create(Class* object_class);
      
    /**
    * Creates an Object instance for the given object_class and invokes its constructor.
    * This should only be used while interpreting as marble code is run.
    */
    static std::shared_ptr<Object> create(Interpreter* interpreter, Class* object_class, std::vector<Value> constructor_values);

    /**
    * Creates an Object instance for the given object_class and invokes its constructor.
    * This should only be used while interpreting as marble code is run.
    */
    static std::shared_ptr<Object> create(Interpreter* interpreter, std::string object_class, std::vector<Value> constructor_values);

    virtual void registerVariable(Variable* variable);

    /**
     * Gets the class of this object
     */
    Class* getClass();

    /**
     * Gets the parent class of this object with the given name.
     * 
     * For example you might want the Animal class for the dog Object.
     *  Object
     *      Animal
     *          Dog
     * 
     * Dog dog = new Dog();
     * dog.getClass("Animal"); 
     */
    Class* getClass(std::string name);
    
    /**
    * Returns true if this Object is currently having code run on it using the runThis method. 
    */
    bool isRunning();

    std::shared_ptr<Object> newInstance();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    void runThis(std::function<void()> function, SystemHandler* handler, Class* c=NULL, OBJECT_ACCESS_TYPE access_type=OBJECT_ACCESS_TYPE_OBJECT_ACCESS, Scope* accessors_scope=NULL);
    virtual void onEnterScope(bool tell_parents);
    virtual void onLeaveScope(bool tell_parents);
private:
    void newRun();
    void finishRun();
    SystemHandler* sys_handler;
    bool is_running;
    std::vector<bool> run_stack;
    Class* c;
};

#endif
