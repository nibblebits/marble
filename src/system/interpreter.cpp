#include <iostream>
#include <memory.h>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include "interpreter.h"
#include "splitter.h"
#include "lexer.h"
#include "parser.h"
#include "nodes.h"
#include "debug.h"
#include "exceptions/IOException.h"
Interpreter::Interpreter()
{

}

Interpreter::~Interpreter()
{

}

void Interpreter::output(const char* data)
{
    /* Here a special output function would be called 
     * instead of direct I/O. This would allow the user of the Marble library to determine how output should be handled. For example if this is running on a standalone application it should output to the console, or if its a web application it should output to the web client of the browsing user.*/
	std::cout << data << std::endl;
}

void Interpreter::run(const char* code)
{
    Lexer lexer;
    lexer.setInput(code, strlen(code));
    Token* root_token = lexer.lex();
	Token* token = root_token;
	while(token != NULL)
	{
		std::cout << token->getType() << ": " << token->getValue().svalue << std::endl;
		token = token->next;	
	}

	Parser parser;
	Node* current_node = parser.parse(root_token);
	while(current_node != NULL)
	{
		if (current_node->getType() == NODE_TYPE_VARIABLE_DECLARATION)
		{
			Varnode* vnode = (Varnode*)(current_node);
			Node* value_node = vnode->value;
			// Ok lets output this value node
			Debug::PrintValueForNode(value_node);
		}
		current_node = current_node->next;
	}
}

void Interpreter::runScript(const char* filename)
{
   // Lets load this script
   FILE* file = fopen(filename, "r");
   if (!file)
   {
     throw IOException("Failed to open file: " + std::string(filename));
   }

   if(fseek(file, 0, SEEK_END) != 0)
   {
     throw IOException("Failed to seek to the end of the file: " + std::string(filename));
   }

   long data_len = ftell(file);
   rewind(file);
   char* data = new char[data_len];
   fread(data, data_len, 1, file);

   Splitter splitter;
   splitter.setData(data, data_len);
   
   split split;
   while(splitter.split(&split))
   {
	// Output the data
    if (split.has_data)
    {
        char* output_data = new char[split.output.size+1];
        memcpy(output_data, split.output.data, split.output.size);
        output_data[split.output.size] = 0;
        output(output_data);
        delete output_data;
    }
	
	// Run the code
    if (split.has_code)
    {
	    char* code_data = new char[split.code.size+1];
	    memcpy(code_data, split.code.data, split.code.size);
	    code_data[split.code.size] = 0;
	    run(code_data);
	    delete code_data;
    }
   }

    // Close and clean up
    fclose(file);
    delete data;
}

