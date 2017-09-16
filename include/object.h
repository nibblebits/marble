#ifndef OBJECT_H
#define OBJECT_H
class ObjectManager;
class Object
{
public:
    Object(ObjectManager* objectManager);
    virtual ~Object();
    void decrementUsage();
    void incrementUsage();
    int usage;
    
private:
    ObjectManager* objectManager;
};

#endif
