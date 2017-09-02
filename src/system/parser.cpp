#include "parser.h"
#include <iostream>
struct order_of_operation o_of_operation[] =
{
    "=", 0, RIGHT_TO_LEFT,
    "+=", 0, RIGHT_TO_LEFT,
    "-=", 0, RIGHT_TO_LEFT,
    "*=", 0, RIGHT_TO_LEFT,
    "/=", 0, RIGHT_TO_LEFT,
    "%=", 0, RIGHT_TO_LEFT,
    "^=", 0, RIGHT_TO_LEFT,
    "&=", 0, RIGHT_TO_LEFT,
    "<<=", 0, RIGHT_TO_LEFT,
    ">>=", 0, RIGHT_TO_LEFT,
    "<<", 0, LEFT_TO_RIGHT,
    ">>", 0, LEFT_TO_RIGHT,
    "<", 1, NON_ASSOCIATIVE,
    ">", 1, NON_ASSOCIATIVE,
    "<=", 1, NON_ASSOCIATIVE,
    ">=", 1, NON_ASSOCIATIVE,
    "==", 2, NON_ASSOCIATIVE,
    "!=", 2, NON_ASSOCIATIVE,
    "&", 3, LEFT_TO_RIGHT,
    "^", 4, LEFT_TO_RIGHT,
    "|", 5, LEFT_TO_RIGHT,
    "+", 6, LEFT_TO_RIGHT,
    "-", 6, LEFT_TO_RIGHT,
    "*", 7, LEFT_TO_RIGHT,
    "/", 7, LEFT_TO_RIGHT,
    "%", 7, LEFT_TO_RIGHT
};


Parser::Parser()
{
    this->root_node = NULL;
    this->current_node = NULL;
    this->current_token = NULL;
}
Parser::~Parser()
{
}


bool Parser::is_datatype(std::string str)
{
    return str == "number" || str == "char" || str == "string" || str == "bool";
}

bool Parser::legal_value(Token* token)
{
    int token_type = token->getType();
    return token != NULL && (token_type == TOKEN_TYPE_IDENTIFIER || token_type == TOKEN_TYPE_STRING || token_type == TOKEN_TYPE_NUMBER);
}


void Parser::parse_error(std::string message)
{
    throw std::logic_error(message);
}

void Parser::ensure_type(Token* token, int expected_type)
{
    int token_type = token->getType();
    if (token_type != expected_type)
    {
        parse_error("Expecting a " + std::to_string(expected_type) + " but a " + std::to_string(token_type) + " was provided");
    }
}

void Parser::push_node(Node* node)
{
    if (this->current_node == NULL)
    {
        this->root_node = node;
        this->current_node = node;
    }
    else
    {
        this->current_node->next = node;
        this->current_node = node;
    }
}

Node* Parser::getLiteralNode(Token* token)
{
    LiteralNode* literal_node = (LiteralNode*) factory.createNode(NODE_TYPE_LITERAL);
    literal_node->value = std::atof(token->value.c_str());
    return literal_node;
}

Node* Parser::getIdentifierNode(Token* token)
{
    IdentifierNode* identifier_node = (IdentifierNode*) factory.createNode(NODE_TYPE_IDENTIFIER);
    identifier_node->value = token->value;
    return identifier_node;
}

Node* Parser::getKeywordNode(Token* token)
{
    KeywordNode* keyword_node = (KeywordNode*) factory.createNode(NODE_TYPE_KEYWORD);
    keyword_node->value = token->value;
    return keyword_node;
}

Node* Parser::getStringNode(Token* token)
{
    StringNode* string_node = (StringNode*) factory.createNode(NODE_TYPE_STRING);
    string_node->value = token->value;
    return string_node;
}

Node* Parser::convertToSingleNode(Token* token)
{
    int tokenType = token->getType();
    if (token->isLiteral())
    {
        return getLiteralNode(token);
    }
    else if(token->isIdentifier())
    {
        return getIdentifierNode(token);
    }
    else if(token->isKeyword())
    {
        return getKeywordNode(token);
    }
    else if(token->isString())
    {
        return getStringNode(token);
    }

    throw std::logic_error("The single \"token\" provided cannot be converted to a node");
}

Token* Parser::peek(int ahead)
{
    return peek(this->current_token, ahead);
}

Token* Parser::peek()
{
    return this->current_token;
}

Token* Parser::peek(Token* token, int ahead)
{
    // Ok lets peek ahead and return the node, we will return NULL if there is nothing to peak ahead to.
    Token* token_to_return = token;
    for (int i = 0; i < ahead; i++)
    {
        if (token_to_return->next == NULL)
            return NULL;

        token_to_return = token_to_return->next;
    }
    return token_to_return;

}

bool Parser::first_op_has_priority(std::string op1, std::string op2)
{
    int op1_priority = get_priority_for_op(op1);
    int op2_priority = get_priority_for_op(op2);

    return op1_priority >= op2_priority;
}

int Parser::get_priority_for_op(std::string op)
{
    struct order_of_operation* operation = get_order_of_operation(op);
    if (operation != NULL)
    {
        return operation->priority;
    }
    return 0;
}

Token* Parser::next()
{
    Token* next_token = this->current_token;
    if (next_token == NULL)
    {
        return NULL;
    }

    this->current_token = next_token->next;
    return next_token;
}


void Parser::parse_variable_declaration()
{
    // Types should be treated as an expression due to the senario "class.other_class var_name"
    parse_expression();
    Node* data_type_node = pop_node();
    Token* var_name_token = next();
    if (!var_name_token->isIdentifier())
    {
        parse_error("Expecting a variable name");
    }

    VarNode* var_node = (VarNode*) factory.createNode(NODE_TYPE_VARIABLE_DECLARATION);
    var_node->type = data_type_node;
    var_node->name = var_name_token->value;
    Token* token_ahead = peek();
    if (token_ahead->isOperator("="))
    {
        // We don't need the equals operator anymore
        next();
        parse_expression();
        var_node->value = pop_node();
    }
    push_node(var_node);

}

void Parser::parse_function_call()
{  
    // Let's parse the function name
    parse_single_token();
    ExpressionInterpretableNode* dest_node = (ExpressionInterpretableNode*) pop_node();
    FunctionCallNode* func_call_node = (FunctionCallNode*) factory.createNode(NODE_TYPE_FUNCTION_CALL);
    func_call_node->dest = dest_node;
    // Now that we have the node lets parse the arguments
    parse_arguments(&func_call_node->arguments);
    push_node(func_call_node);
}

void Parser::parse_arguments(std::vector<ExpressionInterpretableNode*>* argument_nodes)
{
    // Ok lets ignore the "(" bracket
    if(!next()->isSymbol("("))
    {
        parse_error("Expecting a \"(\" symbol for arguments");
    }

    // Parse the arguments
    while(1)
    {    
        parse_expression();
        ExpressionInterpretableNode* argument = (ExpressionInterpretableNode*) pop_node();
        argument_nodes->push_back(argument);
        
        if (!peek()->isSymbol(","))
            break;
        // Ignore the comma
        next();
    }

    // and ignore the ")" bracket
    if(!next()->isSymbol(")"))
    {
        parse_error("Expecting a \")\" symbol for arguments");
    }
}
void Parser::parse_single_token()
{
    Node* node = convertToSingleNode(next());
    push_node(node);
}

void Parser::parse_value()
{
    Token* token = peek();
    Node* node = NULL;
    // Do we have a nested expression here?
    if (token->isSymbol("("))
    {
        // Let's get rid of the "(" symbol ready for parse_expression
        next();
        // Yes we have an expression lets process it
        parse_expression();

        // Now we must get rid of the expression terminator ")"
        token = next();
        if (!token->isSymbol(")"))
        {
            parse_error("Expecting an expression terminator for the given expression");
        }

        node = pop_node();
    }
    else if(token->isIdentifier() && peek(1)->isSymbol("("))
    {
        /* We only care if there is an identifier and a left bracket rather than a full expression such as a.b.c(); The reason
         * for this is that this method "parse_value" should only handle single entity values and not expressions. */
        parse_function_call();
        node = pop_node();
    }
    else if(token->isOperator("-"))
    {
        // We are handling a negative here, e.g number a = -b; number b = -50;
        parse_negated_expression();
        node = pop_node();
    }
    else
    {
        parse_single_token();
        node = pop_node();
    }
    push_node(node);
}


void Parser::parse_semicolon()
{
    Token*  token = next();
    if (token == NULL || !token->isSymbol(";"))
    {
        parse_error("Expecting a semicolon");
    }
}

Node* Parser::get_node_before_last()
{
    Node* node = this->root_node;
    while(node != NULL)
    {
        if (node->next != NULL && node->next->next == NULL)
        {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

Node* Parser::pop_node()
{
    Node* node = get_node_before_last();
    Node* node_to_return = this->current_node;
    if (node != NULL)
    {
        node->next = NULL;
    }

    this->current_node = node;
    return node_to_return;
}

struct order_of_operation* Parser::get_order_of_operation(std::string op)
{
    int size = sizeof (o_of_operation) / sizeof (struct order_of_operation);
    for (int i = 0; i < size; i++)
    {
        struct order_of_operation* ooo = &o_of_operation[i];
        if (ooo->op == op)
        {
            return ooo;
        }
    }

    // Priority is not registered for this operator? Lets just return the default priority.
    return 0;
}


void Parser::do_roltl(ExpressionInterpretableNode** left_pp, ExpressionInterpretableNode** right_pp, std::string& op)
{
    ExpNode* left_exp = (ExpNode*) *left_pp;
    ExpressionInterpretableNode* right = *right_pp;

    ExpressionInterpretableNode* right_of_left = left_exp->right;
    ExpNode* exp_node = (ExpNode*) factory.createNode(NODE_TYPE_EXPRESSION);
    exp_node->left = right_of_left;
    exp_node->right = right;
    exp_node->op = op;
    *left_pp = left_exp->left;
    *right_pp = exp_node;
    op = left_exp->op;
}

void Parser::handle_priority(ExpressionInterpretableNode** left_pp, ExpressionInterpretableNode** right_pp, std::string& op)
{
    ExpressionInterpretableNode* left = *left_pp;
    ExpNode* left_exp = (ExpNode*) *left_pp;
    ExpressionInterpretableNode* right = *right_pp;
      struct order_of_operation* left_op = get_order_of_operation(left_exp->op);
      switch(left_op->associativity)
      {
          case LEFT_TO_RIGHT:
          {
              if (!first_op_has_priority(left_exp->op, op))
              {
                do_roltl(left_pp, right_pp, op);
              }
          }
          break;
          default:
             throw std::logic_error("void Parser::parse_expression(): Unexpected associativity for handle_priority; Only LEFT_TO_RIGHT is handled here.");
      }
}

void Parser::parse_expression()
{
    parse_expression_part();
    Token* peeked_token = peek();
    if (peeked_token != NULL && peeked_token->isOperator())
    {
        // Lets remove the operator from the token stream
        std::string op = next()->value;

        // We now need the last expression as it needs to become our left parameter
        ExpressionInterpretableNode* left = (ExpressionInterpretableNode*) pop_node();
        ExpNode* left_exp = (ExpNode*)(left);

        // We got more to go!
        parse_expression();

        ExpressionInterpretableNode* right = (ExpressionInterpretableNode*) pop_node();
        handle_priority(&left, &right, op);

        ExpNode* exp_node = (ExpNode*) factory.createNode(NODE_TYPE_EXPRESSION);
        exp_node->left = left;
        exp_node->right = right;
        exp_node->op = op;
        push_node(exp_node);
    }
}

void Parser::parse_expression_part()
{
    // Parse the left value
    parse_value();
    ExpressionInterpretableNode* exp_left = (ExpressionInterpretableNode*) pop_node();
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) exp_left;
    Token* peeked_token = peek();
    if (peeked_token != NULL)
    {
        if (peeked_token->isOperator())
        {
            // We have a right part of the expression "l + r"
            std::string op = next()->value;
            struct order_of_operation* operation = get_order_of_operation(op);
            // Left to right associativity should be treated as an expression a = 5 * 5, a = (5 * 5)
            if (operation->associativity == RIGHT_TO_LEFT)
            {
                parse_expression();
            } 
            else 
            {
                // We are non RIGHT_TO_LEFT associativity therefore we expect only a value
                parse_value();
            }
            ExpressionInterpretableNode* exp_right = (ExpressionInterpretableNode*) pop_node();
            ExpNode* exp_node = (ExpNode*) factory.createNode(NODE_TYPE_EXPRESSION);
            exp_node->left = exp_left;
            exp_node->right = exp_right;
            exp_node->op = op;
            node = exp_node;
        }
    }
    push_node(node);

}

void Parser::parse_negated_expression()
{
    // Lets get rid of the negative operator we don't want it anymore
    if(!next()->isOperator("-"))
    {
        parse_error("Expecting a negation symbol for negated expressions. This is a bug and should be reported thank you.");
    }
    
    // The negated expression
    parse_expression();
    
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
    
    NegNode* neg_node = (NegNode*) factory.createNode(NODE_TYPE_NEGATIVE);
    neg_node->node = node;
    push_node(neg_node);
}


void Parser::global_parse()
{
    std::string keyword_name = this->current_token->getValue();
    // Temporary solution, this wont cut it.
    if (is_datatype(keyword_name))
    {
        // Either this is a function or a variable declaration
        Token* peeked_token = peek(this->current_token, 2);
        // If the peeked token is a symbol then this must be a function, e.g number a() { } otherwise its a variable declaration
        if (peeked_token != NULL && peeked_token->isSymbol("("))
        {
            //	parse_function();
        }
        else
        {
            parse_variable_declaration();
            parse_semicolon();
        }
    }
    else if(this->current_token->isIdentifier())
    {
        /* The token is an identifier so this can be treated as an expression as it is one of the following
         * 1. Representing a variable
         * 2. An assignment
         * 3. A function call
         */
        parse_expression();
        parse_semicolon();
    }
    else
    {
        parse_error("That is not legal");
    }
}

Node* Parser::parse(Token* root_token)
{
    this->root_node = NULL;
    this->current_node = NULL;
    this->current_token = root_token;
    while(this->current_token != NULL)
    {
        global_parse();
    }
    return this->root_node;
}


