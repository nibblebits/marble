#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>
class Scope;
class Object;
class Interpreter;
class ObjectManager
{
public:
    ObjectManager(Interpreter* interpreter);
    virtual ~ObjectManager();
    bool isSafeToDelete(Object* object, Scope* ignored_scope);
    void registerObject(Object* object);
    void clean(Scope* scope);
private:
    Interpreter* interpreter;
    std::vector<Object*> objects;
};

#endif
