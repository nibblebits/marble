#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include "nodes.h"
#include "statics.h"
#include "token.h"
#include "nodefactory.h"
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
    void parse_single_token();
    void parse_value();
    void parse_semicolon();
    void push_node(Node* node);
    Node* pop_node();
    Node* get_node_before_last();
    Node* convertToSingleNode(Token* token);
    Node* getLiteralNode(Token* token);
    Node* getIdentifierNode(Token*token);
    Node* getKeywordNode(Token* token);
    void parse_function_call(ExpressionInterpretableNode* dest_node);
    void parse_arguments(std::vector<ExpressionInterpretableNode*>* argument_nodes);

    Token* next();
    Token* peek(int ahead);
    Token* peek();
    Token* peek(Token* token, int ahead);

    bool first_op_has_priority(std::string op1, std::string op2);
    int get_priority_for_op(std::string op);

    NodeFactory factory;
    Token* current_token;
    Node* root_node;
    Node* current_node;
};
#endif
