#ifndef IOPERMISSION_H
#define IOPERMISSION_H

#include "permissionobject.h"
/**
 * The permission class that is used for input output permission that allows or disallows output to the terminal, or web client, or something else.
 */
class IOPermission : public PermissionObject
{
    public:
        IOPermission(Class* c);
        virtual ~IOPermission();
        virtual std::shared_ptr<Object> newInstance(Class* c);
        // Permission properties
        bool output_allowed;
        bool input_allowed;
};

#endif