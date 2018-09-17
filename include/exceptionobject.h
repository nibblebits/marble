
/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef EXCEPTIONOBJECT_H
#define EXCEPTIONOBJECT_H

#include <memory>
#include <string>
#include "object.h"

class Interpreter;
class ThrowNode;
class ExceptionObject : public Object
{
public:
    ExceptionObject(Class* c);
    virtual ~ExceptionObject();
    
    /**
    * Sets the stack log for this exception. This will be used to generate the stack trace when calling getStackTrace()*
    */
    void setStackLog(std::vector<struct stack_log_part> stack_log);
    /**
    
    * Used to set the throw node that threw this exception object. */
    void setThrowNode(ThrowNode* throwNode);

    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    /**
    * Gets a stack trace based on the stack log provided
    */
    std::string getStackTrace();

    /**
     * Sets the message that was thrown with this exception
     */
    void setMessage(std::string message);
    
    /**
     * Returns the message thrown with this exception
     */
    std::string getMessage();
private:
    std::vector<struct stack_log_part> stack_log;
    ThrowNode* throwNode;
    std::string message;
    std::string trace;
};

#endif
