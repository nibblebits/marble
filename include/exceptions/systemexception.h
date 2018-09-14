#ifndef SYSTEMEXCEPTION_H
#define SYSTEMEXCEPTION_H
#include <memory>
#include <string>
#include <vector>
struct stack_log_part;
class Object;
class ExceptionObject;
class SystemException
{
public:
    /**
     * \param object The exception object that this SystemException is related to
     * \param message The exception message to attach to this SystemException
     * \param log_parts The current stack trace log. Use interpreter->getStackTraceLog();
     */
    SystemException(std::shared_ptr<ExceptionObject> object, std::string message, std::vector<struct stack_log_part> log_parts);
    /**
     * \param object The exception object that this SystemException is related to
     */
    SystemException(std::shared_ptr<ExceptionObject> object);
    
    virtual ~SystemException();
    std::shared_ptr<ExceptionObject> getObject();
private:
    std::shared_ptr<ExceptionObject> object;
};
#endif
