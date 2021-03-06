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

#ifndef STATICS_H
#define STATICS_H
static const char* MARBLE_OPEN_TAG = "<marble>";
static const char* MARBLE_CLOSING_TAG = "</marble>";

enum
{
    SYSTEM_HANDLER_VALIDATOR,
    SYSTEM_HANDLER_INTERPRETER,
    SYSTEM_HANDLER_MODULE_SYSTEM,
    SYSTEM_HANDLER_BASE_SYSTEM_HANDLER
};

enum
{
    MODULE_TYPE_MARBLE_LIBRARY = 0x01
};

enum
{
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_SYMBOL
};

enum
{
    NODE_TYPE_VARIABLE_DECLARATION,
    NODE_TYPE_EXPRESSION,
    NODE_TYPE_LITERAL,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_STRING,
    NODE_TYPE_KEYWORD,
    NODE_TYPE_FUNCTION_CALL,
    NODE_TYPE_NEGATIVE,
    NODE_TYPE_IF_STMT,
    NODE_TYPE_BODY,
    NODE_TYPE_CAST,
    NODE_TYPE_ARRAY,
    NODE_TYPE_NEW,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_RETURN,
    NODE_TYPE_CLASS,
    NODE_TYPE_TRY,
    NODE_TYPE_THROW,
    NODE_TYPE_WHILE,
    NODE_TYPE_BREAK,
    NODE_TYPE_CONTINUE,
    NODE_TYPE_DO_WHILE,
    NODE_TYPE_FOR,
    NODE_TYPE_LIST,
    NODE_TYPE_INCLUDE,
    NODE_TYPE_INCLUDE_ONCE,
    NODE_TYPE_PERMISSION_NODE,
    NODE_TYPE_NULL,
    NODE_TYPE_REQUIRE,
    NODE_TYPE_BOOLEAN_LITERAL,
    NODE_TYPE_OUTPUT_NODE,
    NODE_TYPE_LIMIT_SCOPE,
    NODE_TYPE_OUTPUT_FILTER,
    NODE_TYPE_NOT
};

enum
{
    FUNCTION_TYPE_WRITTEN,
    FUNCTION_TYPE_NATIVE,
    FUNCTION_TYPE_GROUPED,
    FUNCTION_TYPE_PURE
};

// This enum should be aligned with the value enum so that using either will match with the other.
enum
{
    VARIABLE_TYPE_NUMBER,
    VARIABLE_TYPE_STRING,
    VARIABLE_TYPE_OBJECT,
    VARIABLE_TYPE_VOID,
    VARIABLE_TYPE_BOOLEAN
};

enum
{
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_STRING,
    VALUE_TYPE_OBJECT,
    VALUE_TYPE_VOID,
    VALUE_TYPE_ARRAY
};

// Class register rules
enum
{
    CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER = 0x01
};

enum
{
    KEEP_SCOPE = 0x01
};

// Bit masking is used for evaluation types
enum
{
    EVALUATION_TYPE_DATATYPE = 0x01,
    EVALUATION_TYPE_VARIABLE = 0x02,
    // Signifies a variable should be resolved if possible
    EVALUATION_FROM_VARIABLE = 0x04,

};

enum
{
    MODIFIER_ACCESS_PRIVATE,
    MODIFIER_ACCESS_PROTECTED,
    MODIFIER_ACCESS_PUBLIC
};



enum
{
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    SELF_PRIORITY
};


// For the lexer
enum
{
    IS_CHARACTER = 0x01,
    IS_NUMBER = 0x02,
    IS_OPERATOR = 0x04,
    IS_SYMBOL = 0x10,
    IS_WHITESPACE = 0x20
};

// Rules for the parser
enum
{
    RULE_PARSE_CASTING = 0x01,
    RULE_PARSE_ARRAY = 0x02,
};

// The possible types of breaking
enum
{   
    BREAK_TYPE_NO_BREAK,
    BREAK_TYPE_BREAK,
    BREAK_TYPE_CONTINUE
};

enum
{
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

enum
{
    OBJECT_ACCESS_TYPE_CLASS_SCAN,
    OBJECT_ACCESS_TYPE_OBJECT_ACCESS
};

enum
{
    SYSTEM_HANDLER_NO_PARENT_BASE_CLASS_LINK = 0x01
};


enum
{
    NODE_LISTENER_ACTION_CARRY_ON,
    NODE_LISTENER_ACTION_LEAVE,
    NODE_LISTENER_ACTION_IGNORE_NODE
};
enum
{
    // Use this state if you want to test only the function declaration
    EXTRA_STATE_FUNCTION_DECLARATION_ONLY = 0x01,
    // Use this state if you do not want to throw an error if the function already exists for when you test the function node
    EXTRA_STATE_ALLOW_FUNCTION_TO_EXIST = 0x02
};

enum
{
    RUN_OK,
    RUN_TIMED_OUT
};
typedef int SYSTEM_HANDLER_TYPE;
typedef int MODULE_TYPE;
typedef int TOKEN_TYPE;
typedef int NODE_TYPE;
typedef int VALUE_TYPE;
typedef int SCOPE_PROPERTIES;
typedef int VARIABLE_TYPE;
typedef int EVALUATION_TYPE;
typedef int FUNCTION_TYPE;
typedef int LOG_TYPE;
typedef int MODIFIER_ACCESS;
typedef int CLASS_REGISTER_RULES;
typedef int BREAK_TYPE;
typedef int OBJECT_ACCESS_TYPE;
typedef int SYSTEM_HANDLER_RULES;
typedef int NODE_LISTENER_ACTION;
typedef int EXTRA_STATE;
typedef int RUN_STATE;

#endif
