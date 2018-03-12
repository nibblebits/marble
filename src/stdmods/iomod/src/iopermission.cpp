#include "iopermission.h"
IOPermission::IOPermission(Class* c) : PermissionObject(c)
{
     this->output_allowed = false;
     this->input_allowed = false;
}

IOPermission::~IOPermission()
{

}

std::shared_ptr<Object> IOPermission::newInstance(Class* c)
{
    return std::make_shared<IOPermission>(c);
}