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
    "||", 1, LEFT_TO_RIGHT,
    "&&", 2, LEFT_TO_RIGHT,
    "|", 3, LEFT_TO_RIGHT,
    "^", 4, LEFT_TO_RIGHT,
    "&", 5, LEFT_TO_RIGHT,
    "==", 6, LEFT_TO_RIGHT,
    "!=", 6, LEFT_TO_RIGHT,
    "<", 7, LEFT_TO_RIGHT,
    "<=", 7, LEFT_TO_RIGHT,
    ">", 7, LEFT_TO_RIGHT,
    ">=", 7, LEFT_TO_RIGHT,
    "<<", 8, LEFT_TO_RIGHT,
    ">>", 8, LEFT_TO_RIGHT,
    "+", 9, LEFT_TO_RIGHT,
    "-", 9, LEFT_TO_RIGHT,
    "*", 10, LEFT_TO_RIGHT,
    "/", 10, LEFT_TO_RIGHT,
    "%", 10, LEFT_TO_RIGHT,
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
    
    throw std::logic_error("The single \"token\" provided cannot be converted to a node. Type: " + std::to_string(token->type) + " = " + token->value);
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

Token* Parser::get_identifier_token(std::string error_msg)
{
     Token* token = next();
     if (!token->isIdentifier())
     {
        parse_error(error_msg);
     }
     return token;
}

void Parser::parse_variable_declaration()
{
    // Types should be treated as an expression due to the senario "class.other_class var_name"
    parse_expression();
    Node* data_type_node = pop_node();
    int array_dimensions = 0;
    while (peek()->isSymbol("["))
    {
        // This variable declaration is an array
        next();
        if (!next()->isSymbol("]"))
        {
            parse_error("symbol \"[\" was provided but failed to provide symbol \"]\"");
        }
        array_dimensions+=1;
    }
    Token* var_name_token = get_identifier_token("Expecting a variable name for variable declaration");
    VarNode* var_node = (VarNode*) factory.createNode(NODE_TYPE_VARIABLE_DECLARATION);
    var_node->type = data_type_node;
    var_node->name = var_name_token->value;
    var_node->dimensions = array_dimensions;
    Token* token_ahead = peek();
    if (token_ahead->isOperator("="))
    {
        // We don't need the equals operator anymore
        next();
        parse_expression_for_value();
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

    // Parse the arguments (if any)
    if (!peek()->isSymbol(")"))
    {
        while(1)
        {   
            parse_expression_for_value();
            ExpressionInterpretableNode* argument = (ExpressionInterpretableNode*) pop_node();
            argument_nodes->push_back(argument);
            
            if (!peek()->isSymbol(","))
                break;
            // Ignore the comma
            next();
        }
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

void Parser::parse_value(int rules)
{
    Token* token = peek();
    Node* node = NULL;
    // Do we have a nested expression here?
    if (token->isSymbol("("))
    {
        // Let's get rid of the "(" symbol ready for parse_expression
        next();
        // Yes we have an expression lets process it
        parse_expression(rules);

        // Now we must get rid of the expression terminator ")"
        token = next();
        if (!token->isSymbol(")"))
        {
            parse_error("Expecting an expression terminator for the given expression");
        }

        node = pop_node();
        
        if ((rules & RULE_PARSE_CASTING) && peek()->isCastableType())
        {
            // This expression is actually a cast
            parse_cast(node);
            node = pop_node();
        }
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
	    parse_negative_expression();
        node = pop_node();
    }
    else if(token->isKeyword("new"))
    {
        // This is for a new statement e.g new number[20];
        parse_new();
        node = pop_node();
    }
    else
    {
        parse_single_token();
        node = pop_node();
    }
    
    if (rules & RULE_PARSE_ARRAY)
    {
        if (peek()->isSymbol("["))
        {
            // Ok we have array access here
            parse_array(node);
            node = pop_node();
        }
    }
    push_node(node);
}

void Parser::parse_cast(Node* casting_to)
{   
    // Ok now lets get what we are casting from
    parse_value(RULE_PARSE_CASTING | RULE_PARSE_ARRAY);
    ExpressionInterpretableNode* to_cast = (ExpressionInterpretableNode*) pop_node();
    
    CastNode* node = (CastNode*) factory.createNode(NODE_TYPE_CAST);
    node->casting_to = casting_to;
    node->to_cast = to_cast;
    push_node(node);
}

/**
* The parse_array method will pop off the node prior to its self and that will become its next element.
* E.g func()[0] would be
* ARRAY
*      0
*      func()
*/
void Parser::parse_array(Node* related_node)
{
    if (!next()->isSymbol("["))
    {
        parse_error("Expecting a left bracket");
    }
    
    parse_expression_for_value();
    
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
    
    if (!next()->isSymbol("]"))
    {
        parse_error("Expecting a right bracket");
    }
    
    ArrayNode* array_node = (ArrayNode*)factory.createNode(NODE_TYPE_ARRAY);
    array_node->index_node = node;
    array_node->next_element = related_node;
    push_node(array_node);
    
    // Before returning lets see if we have anymore arrays we can process
    if (peek()->isSymbol("["))
    {
        // This is a nested array
        parse_array(pop_node());
    }
    
}

void Parser::parse_new()
{
    if(!next()->isKeyword("new"))
    {
        parse_error("Expecting a new keyword for creating new instances. This is a interpreter bug please report this");
    }
    
    parse_expression_for_value();
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
    
    NewNode* new_node = (NewNode*) factory.createNode(NODE_TYPE_NEW);
    new_node->exp = node;
    push_node(new_node);
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

void Parser::parse_expression_for_value(int extra_rules)
{
    parse_expression(RULE_PARSE_CASTING | RULE_PARSE_ARRAY | extra_rules);
}

void Parser::parse_expression(int rules)
{
    parse_expression_part(rules);
    Token* peeked_token = peek();
    if (peeked_token == NULL)
        return;
        
    if (peeked_token->isOperator())
    {
        // Lets remove the operator from the token stream
        std::string op = next()->value;

        // We now need the last expression as it needs to become our left parameter
        ExpressionInterpretableNode* left = (ExpressionInterpretableNode*) pop_node();
        ExpNode* left_exp = (ExpNode*)(left);

        // We got more to go!
        parse_expression_for_value();

        ExpressionInterpretableNode* right = (ExpressionInterpretableNode*) pop_node();
        handle_priority(&left, &right, op);

        ExpNode* exp_node = (ExpNode*) factory.createNode(NODE_TYPE_EXPRESSION);
        exp_node->left = left;
        exp_node->right = right;
        exp_node->op = op;
        push_node(exp_node);
    }
}

void Parser::parse_expression_part(int rules)
{
    // Parse the left value
    parse_value(rules);
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
                parse_expression_for_value();
            } 
            else 
            {
                // We are non RIGHT_TO_LEFT associativity therefore we expect only a value
                parse_value(rules);
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

void Parser::parse_negative_expression()
{
    // Lets get rid of the negative operator we don't want it anymore
    if(!next()->isOperator("-"))
    {
        parse_error("Expecting a negation symbol for negated expressions. This is a bug and should be reported thank you.");
    }
    
    // The negated expression
    parse_value();
    
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
    
    NegNode* neg_node = (NegNode*) factory.createNode(NODE_TYPE_NEGATIVE);
    neg_node->node = node;
    push_node(neg_node);
}

void Parser::parse_if_stmt()
{
    if (!next()->isKeyword("if"))
    {
        parse_error("Expecting an \"if\" for parsing \"if\" statements. Please report this as it is a bug");
    }
    
    // We are expecting a left bracket
    if (!next()->isSymbol("("))
    {
        parse_error("Expecting an expression for \"if\" statements.");
    }
   
    parse_expression_for_value();
    ExpressionInterpretableNode* exp = (ExpressionInterpretableNode*) pop_node();
    
    if (!next()->isSymbol(")"))
    {
        parse_error("Expecting a right bracket to end the \"if\" statement expression");
    }
    
    parse_body();
    BodyNode* body = (BodyNode*) pop_node();
    
    IfStatementNode* if_stmt = (IfStatementNode*) factory.createNode(NODE_TYPE_IF_STMT);
    if_stmt->exp = exp;
    if_stmt->body = body;
    push_node(if_stmt);
}

void Parser::parse_body()
{
    if (!next()->isSymbol("{"))
    {
        parse_error("Expecting a left bracket \"{\" for given body");
    }
    
    BodyNode* body_node = (BodyNode*) factory.createNode(NODE_TYPE_BODY);
    while(!peek()->isSymbol("}"))
    {
        parse_body_next();
        body_node->addChild(pop_node());
    }
    
    // Lets remove the "}" symbol
    next();
    
    push_node(body_node);
}
void Parser::parse_body_next()
{
    std::string token_value = this->current_token->getValue();
    if (this->current_token->isKeyword() && is_datatype(token_value))
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
    else if(this->current_token->isKeyword("if"))
    {
        parse_if_stmt();
    }
    else if(this->current_token->isIdentifier())
    {
        /* The token is an identifier so this can be treated as an expression as it is one of the following
         * 1. Representing a variable
         * 2. An assignment
         * 3. A function call
         */
        parse_expression_for_value();
        parse_semicolon();
    }
    else
    {
        parse_error("That is not legal: " + token_value);
    }
}
void Parser::global_parse_next()
{
    parse_body_next();
}

Node* Parser::parse(Token* root_token)
{
    this->root_node = NULL;
    this->current_node = NULL;
    this->current_token = root_token;
    while(this->current_token != NULL)
    {
        global_parse_next();
    }
    return this->root_node;
}


