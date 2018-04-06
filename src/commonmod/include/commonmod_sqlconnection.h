#ifndef COMMONMOD_SQLCONNECTION
#define COMMONMOD_SQLCONNECTION

#include "object.h"
/**
 * An abstract pure SQLConnection class. Extend this if you wish to create your own SQL connection
 */

class ModuleSystem;
class CommonModule_SqlDriver;
class CommonModule_SqlConnection : public Object
{
public:
    CommonModule_SqlConnection(Class* c);
    virtual ~CommonModule_SqlConnection();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native SQLConnection functions
    static void SQLConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLConnection_getStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLConnection_getPreparedStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    // The SQLDriver for this connection
    std::shared_ptr<CommonModule_SqlDriver> driver;
};

#endif