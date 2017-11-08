#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "nodes.h"
#include "statics.h"
#include "token.h"
#include "nodefactory.h"
#include "logger.h"

struct order_of_operation
{
    const char* op;
    int priority;
    int associativity;
};

class Parser
{
public:
    Parser(Logger* logger);
    virtual ~Parser();
    Node* parse(Token* root_token);
    Logger* getLogger();
private:
    void global_parse_next();
    bool is_datatype(std::string str);
    bool legal_value(Token* token);
    void parse_error(std::string message);
    void ensure_type(Token* token, int expected_type);
    MODIFIER_ACCESS get_modifier_access_for_string(std::string str);
    Token* get_identifier_token(std::string error_msg);
    
    void parse_variable_declaration();
    void parse_expression_for_value(int extra_rules=0);
    void parse_expression(int rules=0);
    void parse_expression_part(int rules=0);
    void parse_negative_expression();
    void parse_single_token();
    void parse_if_stmt();
    void parse_else();
    void parse_modifier_access();
    void parse_class_body();
    void parse_class_body_next();
    void parse_body();
    void parse_body_next();
    void parse_value(int rules=0);
    void parse_cast(Node* casting_to);
    void parse_array(Node* related_node);
    void parse_new();
    void parse_return();
    void parse_class();
    void parse_semicolon();
    void push_node(Node* node);

    struct order_of_operation* get_order_of_operation(std::string op);
    void handle_priority(ExpressionInterpretableNode** left_pp, ExpressionInterpretableNode** right_pp, std::string& op);
    void do_roltl(ExpressionInterpretableNode** left_pp, ExpressionInterpretableNode** right_pp, std::string& op);
    Node* pop_node();
    Node* get_node_before_last();
    Node* convertToSingleNode(Token* token);
    Node* getLiteralNode(Token* token);
    Node* getIdentifierNode(Token*token);
    Node* getKeywordNode(Token* token);
    Node* getStringNode(Token* token);
    void parse_function_call();
    void parse_arguments(std::vector<ExpressionInterpretableNode*>* argument_nodes);
    void parse_function();
    std::vector<VarNode*> parse_declared_arguments();

    Token* next();
    Token* peek(int ahead);
    Token* peek();
    Token* peek(Token* token, int ahead);

    bool first_op_has_priority(std::string op1, std::string op2);
    int get_priority_for_op(std::string op);
    
    // The current modifier access, e.g public, private, protected
    MODIFIER_ACCESS access;
    
    // A token that has no value and is returned rather than returning NULL
    std::unique_ptr<Token> dummy_token;
    
    Logger* logger;
    NodeFactory factory;
    Token* prev_token;
    Token* current_token;
    Node* root_node;
    Node* current_node;
};
#endif
