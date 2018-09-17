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

#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
#include "filepermission.h"
#include "misc.h"
#include "exceptions/testerror.h"
#include "exceptions/IOException.h"

RequireNode::RequireNode() : InterpretableNode(NODE_TYPE_REQUIRE)
{
    this->required_ast = NULL;
}

RequireNode::~RequireNode()
{
    this->splitter.free();
}


void RequireNode::test(Validator* validator, struct extras extra)
{
    this->code_result = "";
    try
    {
        Interpreter* interpreter = validator->getInterpreter();
        std::string absolute_path = getAbsolutePath(this->filename);
        this->splitter = interpreter->loadScript(absolute_path.c_str());
        split split;
        PosInfo posInfo;
        posInfo.filename = absolute_path;
        posInfo.line = 1;
        posInfo.col = 1;
        
        // Merge the splits to create one final code result
        while(this->splitter.split(&split))
        {
            // Merge the code and data for this split
            this->code_result += interpreter->mergeCodeAndDataForSplit(&split);
        }
        Node* root_node = interpreter->getAST(code_result.c_str(), posInfo);
        this->required_ast = (InterpretableNode*) root_node;
        
        // Test the AST
        validator->validate(this->required_ast);
    }
    catch(TestError& ex)
    {
        throw TestError("Failed to require file: " + this->filename + " " + ex.what());
    }
    catch(IOException& ex)
    {
        throw TestError("There was a problem requiring the file: " + this->filename + ", message: " + ex.what());
    }

}

Value RequireNode::interpret(Interpreter* interpreter, struct extras extra)
{

    std::string absolute_path = getAbsolutePath(this->filename);
    // Before we do anything let's make sure we have permission to load this file
    FilePermission::checkPermissionAllows(interpreter, interpreter->getCurrentScope(), absolute_path.c_str(), "r");

    // Run the nodes for the file that was required
    InterpretableNode* current_node = this->required_ast;
    while(current_node != NULL)
    {
        current_node->interpret(interpreter);
        current_node = (InterpretableNode*) current_node->next;
    }

    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}