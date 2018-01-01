#ifndef MODULESYSTEM_H
#define MODULESYSTEM_H

class Interpreter;
class ModuleSystem
{
public:
    ModuleSystem();
    virtual ~ModuleSystem();
    void setInterpreter(Interpreter* interpreter);
    void loadModule(const char* filename);
private:
    Interpreter* interpreter;

};

#endif