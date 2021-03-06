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
    void ensure_peek_non_dummy(std::string error_message);
    void ensure_type(Token* token, int expected_type);
    MODIFIER_ACCESS get_modifier_access_for_string(std::string str);
    Token* get_identifier_token(std::string error_msg);
    
    /**
     * Parses the amount of array dimensions and returns the amount. For example
     * abc[][] would return 2 abc[] would return 1 abc[][][] would return 3.
     * This method is only applicable for array dimensions this means array blocks without values.
     * 
     * If your looking for abc[10][3] then please look at parse_array
     */
    int parse_array_dimensions();
    void parse_variable_declaration();
    void parse_expression_for_value(int extra_rules=0);
    void parse_expression(int rules=0);
    void parse_negative_expression();
    void parse_not_expression();
    void parse_single_token();
    void parse_if_stmt();
    void parse_else();
    void parse_modifier_access();
    void parse_class_body();
    void parse_class_body_next();
    void parse_body();

     /**
     * Used to parse a series of nodes that represent one of the following:
     * 1. A variable
     * 2. An assignment
     * 3. A function call
     * 4. A variable declaration with an object type
     * 5. An expression
     * 
     * A resulting node is then pushed to the stack with the resulting node being one of the above.
     * 
     * \attention An identifier must follow before this method is called or a parse error will be thrown.
    */
    void parse_vafde_for_identifier();
    void parse_expression_or_variable_declaration();
    void parse_body_next();
    void parse_value(int rules=0);
    void parse_cast(Node* casting_to);
    void parse_array(ExpressionInterpretableNode* related_node);
    void parse_new();
    void parse_return();
    void parse_class();
    void parse_try();
    void parse_throw();
    ExpressionInterpretableNode* parse_while_stmt_exp();
    void parse_while();
    void parse_do_while();
    void parse_include();
    void parse_include_once();
    void parse_require();
    void parse_permission_node();
    void parse_null();
    void parse_boolean();
    void parse_filter_output_node();
    
    /**
     * Parses an output node such as
     * "Hello world"
     * with nothing else
     */
    void parse_output_node();

    /**
     * Parses multiple expressions seperated by commas ",". If the first token seen is a symbol of ";" then an empty ListNode is pushed to the stack.
     * Multi-expressions also allow for variable declarations when calling this method.
     * While traditional expressions "parse_expression()" do not.
     **/
    void parse_multi_expression();
    void parse_for();
    void parse_break();
    void parse_continue();
    void parse_semicolon();
    void push_node(Node* node);

    struct order_of_operation* get_order_of_operation(std::string op);

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
    void parse_limit_scope();
    
    /**
     * If is_in_value == TRUE then we treat it as a value such as parse_value. Otherwise we use parse_body_next.
     */
    void parse_ignore_validation(bool is_in_value);
    void parse_final();
    void parse_pure();
    void parse_pure_class();
    void parse_pure_function();
    void parse_function_declaration();
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
