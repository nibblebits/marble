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

#include "ifstmtnode.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
IfStatementNode::IfStatementNode() : Statement(NODE_TYPE_IF_STMT)
{
    this->exp = NULL;
    this->next = NULL;
}

IfStatementNode::~IfStatementNode()
{

}

void IfStatementNode::test(Validator* validator, struct extras extra)
{
    try
    {
    	//this->exp->test(validator);
   	    this->body->test(validator);
    }
    catch(TestError& ex)
    {
	    throw TestError(ex.getMessage() + " for if statement");
    }
}


Value IfStatementNode::interpret(Interpreter* interpreter, struct extras extra)
{
    if (exp == NULL)
        throw std::logic_error("Expecting an expression");
    
    // Test the expression
    Value result = exp->interpret(interpreter);
    // Do we have a true result?
    if (result.dvalue == 1)
    {
        // We do lets execute
        body->interpret(interpreter);
    }
    else
    {
        // This is the next expression, it will be an else statement or an else if statement so lets interpret it
        if (this->next != NULL)
        {
            this->next->interpret(interpreter);
        }
    }   
    
    Value v;
    return v;
}

void IfStatementNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("If statements cannot be evaluated");
}
