#ifndef SYSTEMEXCEPTION_H
#define SYSTEMEXCEPTION_H
#include <memory>
class Object;
class SystemException
{
public:
    SystemException(std::shared_ptr<Object> object);
    virtual ~SystemException();
    std::shared_ptr<Object> getObject();
private:
    std::shared_ptr<Object> object;
};
#endif
