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
    void registerObject(Object* object);
    void incrementUsage(Object* object);
    void decrementUsage(Object* object);
    void clean();
private:
    Interpreter* interpreter;
    std::vector<Object*> objects;
};

#endif
