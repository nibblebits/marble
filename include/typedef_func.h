#ifndef TYPEDEF_FUNC_H
#define TYPEDEF_FUNC_H

#include <vector>
#include <memory>

class Interpreter;
class Value;
class Object;
class Scope;
typedef std::function<void(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)> NATIVE_FUNCTION_ENTRYPOINT;
#endif