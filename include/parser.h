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
		std::shared_ptr<Node> parse(std::shared_ptr<Token> root_token);
	private:
		void global_parse_keyword();
		bool is_datatype(std::string str);
		bool legal_value(std::shared_ptr<Token> token);
		void parse_error(std::string message);
		void ensure_type(std::shared_ptr<Token> token, int expected_type);
		void parse_variable_declaration();
		void parse_expression();
		void parse_value();
		void push_node(std::shared_ptr<Node> node);
		std::shared_ptr<Node> pop_node();
		std::shared_ptr<Node> get_node_before_last();
		std::shared_ptr<Node> convertToNode(std::shared_ptr<Token> token);
		std::shared_ptr<Node> getLiteralNode(std::shared_ptr<Token> token);
		std::shared_ptr<Node> getIdentifierNode(std::shared_ptr<Token> token);

		std::shared_ptr<Token> next();
		std::shared_ptr<Token> peek(int ahead);
		std::shared_ptr<Token> peek();
		std::shared_ptr<Token> peek(std::shared_ptr<Token> token, int ahead);

		std::shared_ptr<Token> current_token;
		std::shared_ptr<Node> root_node;
		std::shared_ptr<Node> current_node;
};
#endif
