#ifndef STATICS_H
#define STATICS_H
static const char* MARBLE_OPEN_TAG = "<marble>";
static const char* MARBLE_CLOSING_TAG = "</marble>";

enum
{
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_IDENTIFIER,
	TOKEN_TYPE_KEYWORD,
	TOKEN_TYPE_UNKNOWN
};
#endif
