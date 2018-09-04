#ifndef MYSQLMOD_H
#define MYSQLMOD_H
#include "module.h"
class Scope;
class MysqlModule : public Module
{
public:
    MysqlModule();
    virtual ~MysqlModule();
    virtual void Init();
    virtual void unload();
    void newInterpreter(Interpreter* interpreter);
private:
  
};
#endif