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

#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "interpreter.h"
#include <stdexcept>
#include <iostream>
SystemException::SystemException(std::shared_ptr<ExceptionObject> object)
{
    if (object == NULL)
        throw std::logic_error("A null object was provided");
    this->object = object;
}

SystemException::SystemException(std::shared_ptr<ExceptionObject> object, std::string message, std::vector<struct stack_log_part> log_parts) : SystemException(object)
{
    this->object->setMessage(message);
    this->object->setStackLog(log_parts);
}

SystemException::~SystemException()
{

}

std::shared_ptr<ExceptionObject> SystemException::getObject()
{
    return this->object;
}
