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
		void parse_error(std::string message);
		void ensure_type(std::shared_ptr<Token> token, int expected_type);
		void parse_variable_declaration();
		void push_node(std::shared_ptr<Node> node);

		std::shared_ptr<Token> current_token;
		std::shared_ptr<Node> root_node;
		std::shared_ptr<Node> current_node;
};
#endif
