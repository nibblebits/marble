#ifndef STATICS_H
#define STATICS_H
static const char* MARBLE_OPEN_TAG = "<marble>";
static const char* MARBLE_CLOSING_TAG = "</marble>";

enum
{
    SYSTEM_HANDLER_VALIDATOR,
    SYSTEM_HANDLER_INTERPRETER
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
    NODE_TYPE_LIST
};

enum
{
    FUNCTION_TYPE_WRITTEN,
    FUNCTION_TYPE_NATIVE,
    FUNCTION_TYPE_GROUPED
};

// This enum should be aligned with the value enum so that using either will match with the other.
enum
{
    VARIABLE_TYPE_NUMBER,
    VARIABLE_TYPE_STRING,
    VARIABLE_TYPE_OBJECT,
    VARIABLE_TYPE_ARRAY,
    VARIABLE_TYPE_BYTE,
    VARIABLE_TYPE_VOID,
};

enum
{
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_STRING,
    VALUE_TYPE_OBJECT,
    VALUE_TYPE_ARRAY,
    VALUE_TYPE_BYTE
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
    NON_ASSOCIATIVE
};


// For the lexer
enum
{
    IS_CHARACTER,
    IS_NUMBER,
    IS_OPERATOR,
    IS_SYMBOL,
    IS_WHITESPACE
};

// Rules for the parser
enum
{
    RULE_PARSE_CASTING = 0x01,
    RULE_PARSE_ARRAY = 0x02
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

typedef int SYSTEM_HANDLER_TYPE;
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
#endif
