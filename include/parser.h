#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <memory>
#include <string>
#include "nodes.h"
#include "statics.h"
#include "token.h"

class Parser
{
	public:
		Parser();
		virtual ~Parser();
		Node* parse(Token* root_token);
	private:
		void global_parse();
		bool is_datatype(std::string str);
		bool legal_value(Token* token);
		void parse_error(std::string message);
		void ensure_type(Token* token, int expected_type);
		void parse_variable_declaration();
		void parse_expression();
		void parse_expression_part();
		void parse_value();
		void parse_semicolon();
		void push_node(Node* node);
		Node* pop_node();
		Node* get_node_before_last();
		Node* convertToNode(Token* token);
		Node* getLiteralNode(Token* token);
		Node* getIdentifierNode(Token*token);

		Token* next();
		Token* peek(int ahead);
		Token* peek();
		Token* peek(Token* token, int ahead);

		bool first_op_has_priority(std::string op1, std::string op2);
		int get_priority_for_op(std::string op);

		Token* current_token;
		Node* root_node;
		Node* current_node;
};
#endif
