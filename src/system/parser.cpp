#include "parser.h"
#include "datatype.h"
#include "debug.h"
#include <iostream>
struct order_of_operation o_of_operation[] =
{
    "=", 14, RIGHT_TO_LEFT,
    "+=", 14, RIGHT_TO_LEFT,
    "-=", 14, RIGHT_TO_LEFT,
    "*=", 14, RIGHT_TO_LEFT,
    "/=", 14, RIGHT_TO_LEFT,
    "%=", 14, RIGHT_TO_LEFT,
    "^=", 14, RIGHT_TO_LEFT,
    "&=", 14, RIGHT_TO_LEFT,
    "<<=", 14, RIGHT_TO_LEFT,
    ">>=", 14, RIGHT_TO_LEFT,
    "||", 12, LEFT_TO_RIGHT,
    "&&", 11, LEFT_TO_RIGHT,
    "|", 10, LEFT_TO_RIGHT,
    "^", 9, LEFT_TO_RIGHT,
    "&", 8, LEFT_TO_RIGHT,
    "==", 7, LEFT_TO_RIGHT,
    "!=", 7, LEFT_TO_RIGHT,
    "<", 6, LEFT_TO_RIGHT,
    "<=", 6, LEFT_TO_RIGHT,
    ">", 6, LEFT_TO_RIGHT,
    ">=", 6, LEFT_TO_RIGHT,
    "<<", 5, LEFT_TO_RIGHT,
    ">>", 5, LEFT_TO_RIGHT,
    "+", 4, LEFT_TO_RIGHT,
    "-", 4, LEFT_TO_RIGHT,
    "*", 3, LEFT_TO_RIGHT,
    "/", 3, LEFT_TO_RIGHT,
    "%", 3, LEFT_TO_RIGHT,
    ".", 1, LEFT_TO_RIGHT,
};


Parser::Parser(Logger* logger)
{
    this->logger = logger;
    this->root_node = NULL;
    this->current_node = NULL;
    this->prev_token = NULL;
    this->current_token = NULL;
    this->access = MODIFIER_ACCESS_PUBLIC;
    
    PosInfo info;
    info.line = -1;
    info.col = -1;
    info.filename = "DUMMY";
    
    this->dummy_token = std::unique_ptr<Token>(new Token(-1, info));
}
Parser::~Parser()
{
}


bool Parser::is_datatype(std::string str)
{
    return str == "number" || str == "byte" || str == "string" || str == "bool" || str == "void";
}

bool Parser::legal_value(Token* token)
{
    int token_type = token->getType();
    return token != NULL && (token_type == TOKEN_TYPE_IDENTIFIER || token_type == TOKEN_TYPE_STRING || token_type == TOKEN_TYPE_NUMBER);
}


void Parser::parse_error(std::string message)
{
    if (this->prev_token == NULL)
        throw std::logic_error("No previous token to log error for");
    logger->error(message, this->prev_token->posInfo);
}

void Parser::ensure_peek_non_dummy(std::string error_message)
{
    if (peek() == this->dummy_token.get())
        parse_error(error_message);
}
void Parser::ensure_type(Token* token, int expected_type)
{
    int token_type = token->getType();
    if (token_type != expected_type)
    {
        parse_error("Expecting a " + std::to_string(expected_type) + " but a " + std::to_string(token_type) + " was provided");
    }
}

MODIFIER_ACCESS Parser::get_modifier_access_for_string(std::string str)
{
    MODIFIER_ACCESS access;
    if (str == "private")
    {
        access = MODIFIER_ACCESS_PRIVATE;
    }
    else if(str == "public")
    {
        access = MODIFIER_ACCESS_PUBLIC;
    }
    else if(str == "protected")
    {
        access = MODIFIER_ACCESS_PROTECTED;
    }
    else
    {
        throw std::logic_error("No access for the given string: " + str);
    }
    
    return access;
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
    if (this->current_token == NULL)
        return dummy_token.get();
        
    return this->current_token;
}

Token* Parser::peek(Token* token, int ahead)
{
    // Ok lets peek ahead and return the node, we will return NULL if there is nothing to peak ahead to.
    Token* token_to_return = token;
    for (int i = 0; i < ahead; i++)
    {
        if (token_to_return->next == NULL)
            return dummy_token.get();

        token_to_return = token_to_return->next;
    }
    return token_to_return;

}

bool Parser::first_op_has_priority(std::string op1, std::string op2)
{
    int op1_priority = get_priority_for_op(op1);
    int op2_priority = get_priority_for_op(op2);

    return op1_priority <= op2_priority;
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
    Token* token = this->current_token;
    if (token == NULL)
    {
        throw std::logic_error("Expecting input");
    }
    this->prev_token = token;
    this->current_token = this->current_token->next;
    factory.applyPosition(token->posInfo);
    return token;
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
    parse_value();
    EvaluatingNode* data_type_node = (EvaluatingNode*) pop_node();
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
    var_node->access = access;
    Token* token_ahead = peek();
    if (token_ahead->isOperator("="))
    {
        // We don't need the equals operator anymore
        next();
        parse_expression_for_value();
        var_node->value = (InterpretableNode*) pop_node();
    }
    push_node(var_node);

}

void Parser::parse_function_call()
{  
    // Let's parse the function name
    parse_single_token();
    IdentifierNode* name_node = (IdentifierNode*) pop_node();
    FunctionCallNode* func_call_node = (FunctionCallNode*) factory.createNode(NODE_TYPE_FUNCTION_CALL);
    func_call_node->name = name_node;
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

void Parser::parse_function()
{
    parse_function_declaration();
    FunctionNode* f_node = (FunctionNode*) pop_node();
    parse_body();
    f_node->body = (BodyNode*) pop_node();
    push_node(f_node);
}

void Parser::parse_ignore_validation(bool is_in_value)
{
    if (!next()->isSymbol("@"))
    {
        parse_error("Expecting ignored validations to start with @");
    }

    if (is_in_value)
        parse_value();
    else
        parse_body_next();

    InterpretableNode* inode = (InterpretableNode*) pop_node();
    inode->ignoreValidation();
    push_node(inode);
}

void Parser::parse_final()
{
    if (!next()->isKeyword("final"))
        parse_error("Expecting a final keyword");
    
    if (!peek()->isKeyword("class"))
        parse_error("Only classes can be final");
    
    parse_class();
    ClassNode* c_node = (ClassNode*) pop_node();
    c_node->is_final = true;
    push_node(c_node);
}

void Parser::parse_pure()
{
    if (!next()->isKeyword("pure"))
        parse_error("Expecting a pure keyword for parsing pures");
    
    if (peek()->isKeyword("class"))
        parse_pure_class();
    else if(peek()->isKeyword("function"))
        parse_pure_function();
    else
        parse_error("Invalid pure provided: " + peek()->value);
}

void Parser::parse_pure_class()
{
    parse_class();
    ClassNode* c_node = (ClassNode*) pop_node();
    c_node->is_pure = true;
    push_node(c_node);
}

void Parser::parse_pure_function()
{       
    parse_function_declaration();
    FunctionNode* f_node = (FunctionNode*) pop_node();
    f_node->is_pure = true;
    push_node(f_node);

    // Let's get rid of that semicolon for this pure function.
    parse_semicolon();
}

void Parser::parse_function_declaration()
{
    IdentifierNode* name_node;
    std::vector<VarNode*> args;
    ExpressionInterpretableNode* return_type;
    FunctionNode* function_node;
    if (!next()->isKeyword("function"))
    {
        parse_error("Expecting a function keyword for a function");
    }
    
    if (!peek()->isIdentifier())
    {
        parse_error("Expecting a function name when declaring a function");
    }
    
    parse_single_token();
    name_node = (IdentifierNode*) pop_node();
    if (!next()->isSymbol("("))
    {
        parse_error("Expecting a left bracket for function arguments");
    }
    if (!peek()->isSymbol(")"))
    {
        args = parse_declared_arguments();
    }
    if (!next()->isSymbol(")"))
    {
        parse_error("Expecting a right bracket to end the function arguments");
    }
    
    if (!next()->isSymbol(":"))
    {
        parse_error("Expecting a return type, use : ");
    }
    
    if (!peek()->isIdentifier() && !peek()->isKeyword())
    {
        parse_error("Expecting a valid return type");
    }
    return_type = (ExpressionInterpretableNode*) convertToSingleNode(next());
    function_node = (FunctionNode*) factory.createNode(NODE_TYPE_FUNCTION);
    function_node->name = name_node->value;
    function_node->args = args;
    function_node->return_type = return_type;
    function_node->access = access;
    push_node(function_node);   
}


std::vector<VarNode*> Parser::parse_declared_arguments()
{
    std::vector<VarNode*> args;
    while(1)
    {   
        parse_variable_declaration();
        VarNode* argument = (VarNode*) pop_node();
        args.push_back(argument);
                    
        if (!peek()->isSymbol(","))
            break;
        // Ignore the comma
        next();
    }
    return args;
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
        // Yes we have an expression lets process it.
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
    else if(token->isSymbol("@"))
    {
        /*
         * This is the ignore validation symbol so lets parse it
         */
        parse_ignore_validation(true);
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
            parse_array((ExpressionInterpretableNode*) node);
            node = pop_node();
        }
    }
    push_node(node);
}

void Parser::parse_cast(Node* casting_to)
{   
    // Ok now lets get what we are casting from
    parse_expression_for_value();
    ExpressionInterpretableNode* to_cast = (ExpressionInterpretableNode*) pop_node();
    
    CastNode* node = (CastNode*) factory.createNode(NODE_TYPE_CAST);
    node->casting_to = (EvaluatingNode*) casting_to;
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
void Parser::parse_array(ExpressionInterpretableNode* related_node)
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
        parse_array((ExpressionInterpretableNode*) pop_node());
    }
    
}

void Parser::parse_new()
{
    if(!next()->isKeyword("new"))
    {
        parse_error("Expecting a new keyword for creating new instances. This is a interpreter bug please report this");
    }
    
    parse_expression();
    ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
    NewNode* new_node = (NewNode*) factory.createNode(NODE_TYPE_NEW);
    new_node->type_node = node;
    
    while (peek()->isSymbol("["))
    {
        // This new declaration is handling an array
        next();
        parse_expression_for_value();
        new_node->array_values.push_back((ExpressionInterpretableNode*)pop_node());
        if (!next()->isSymbol("]"))
        {
            parse_error("Expecting a \"]\" symbol to end the array for when creating a new array");
        }
    }
    
    push_node(new_node);
}

void Parser::parse_return()
{
    if (!next()->isKeyword("return"))
    {
        parse_error("Expecting a return keyword for a return statement");
    }
    
    ExpressionInterpretableNode* exp = NULL;
    if (!peek()->isSymbol(";"))
    {
        parse_expression_for_value();
        exp = (ExpressionInterpretableNode*) pop_node();
    }
    ReturnNode* return_node = (ReturnNode*) factory.createNode(NODE_TYPE_RETURN);
    return_node->exp = exp;
    push_node(return_node);
}

void Parser::parse_class()
{
    std::string parent = "";
    if (!next()->isKeyword("class"))
    {
        parse_error("Expecting a class keyword for class definitions");
    }
    
    
    Token* name_token = next();
    if (name_token->type != TOKEN_TYPE_IDENTIFIER)
    {
        parse_error("Expecting a valid class name");
    }
    
    if (peek()->isKeyword("extends"))
    {
        next();
        if (!peek()->isIdentifier())
        {
            parse_error("Expecting a class to extend from when using extends");
        }
        parent = next()->value;
    }
    
    // Now for the class body
    parse_class_body();
    BodyNode* body_node = (BodyNode*) pop_node();
    
    // Now lets create this class node
    ClassNode* class_node = (ClassNode*) factory.createNode(NODE_TYPE_CLASS);
    class_node->name = name_token->value;
    class_node->parent = parent;
    class_node->body = body_node;
    
    // Now we are done with this class lets reset the access to public
    access = MODIFIER_ACCESS_PUBLIC;
    
    push_node(class_node);
}

void Parser::parse_try()
{
    if (!next()->isKeyword("try"))
    {
        parse_error("Expecting a try keyword for try and catch definitions");
    }

    parse_body();
    BodyNode* try_body = (BodyNode*) pop_node();
    if (!next()->isKeyword("catch"))
    {
        parse_error("Try statement but without a catch");
    }
    
    // Now we expect a catch expression variable
    if (!next()->isSymbol("("))
    {
        parse_error("catch's require expressions. try { } catch(Exception ex) { }");
    }
    // A variable declaration is what we need. E.g catch(Variable name)
    parse_variable_declaration();
    VarNode* catch_varnode = (VarNode*) pop_node();
    if (!next()->isSymbol(")"))
    {
        parse_error("catch expression opened but never closed. Missing \")\"");
    }
    
    parse_body();
    BodyNode* catch_body = (BodyNode*) pop_node();
    
    TryNode* try_node = (TryNode*) factory.createNode(NODE_TYPE_TRY);
    try_node->try_body = try_body;
    try_node->catch_varnode = catch_varnode; 
    try_node->catch_body = catch_body;
    push_node(try_node);
}

void Parser::parse_throw()
{
    if (!next()->isKeyword("throw"))
    {
        parse_error("Expecting a throw keyword for try and catch definitions");
    }
    
    // Parse the throw expression
    parse_expression();
    ExpressionInterpretableNode* exp = (ExpressionInterpretableNode*) pop_node();
    ThrowNode* throw_node = (ThrowNode*) factory.createNode(NODE_TYPE_THROW);
    throw_node->exp = exp;
    push_node(throw_node);
}

ExpressionInterpretableNode* Parser::parse_while_stmt_exp()
{
    if (!next()->isKeyword("while"))
    {
        parse_error("Expecting a while keyword for while statements");
    }
    
    if (!next()->isSymbol("("))
    {
        parse_error("Expecting an \"(\" after while");
    }
    
    // Parse the while expression
    parse_expression();
    ExpressionInterpretableNode* exp = (ExpressionInterpretableNode*) pop_node();
    if(!next()->isSymbol(")"))
    {
        parse_error("Expecting an \")\" after while expression");
    }
    
    return exp;
}
void Parser::parse_while()
{
    ExpressionInterpretableNode* exp = parse_while_stmt_exp();
    // Parse the while body
    parse_body();
    BodyNode* body = (BodyNode*) pop_node();
    WhileNode* while_node = (WhileNode*) factory.createNode(NODE_TYPE_WHILE);
    while_node->exp = exp;
    while_node->body = body;
    push_node(while_node);
}

void Parser::parse_do_while()
{
    if (!next()->isKeyword("do"))
    {
        parse_error("Expecting a do keyword for do while statements");
    }
    
    // Parse the do while statement body
    parse_body();
    
    BodyNode* body_node = (BodyNode*) pop_node();
    
    // Parse the while statement
    ExpressionInterpretableNode* exp = parse_while_stmt_exp();
    DoWhileNode* do_while_node = (DoWhileNode*) factory.createNode(NODE_TYPE_DO_WHILE);
    do_while_node->exp = exp;
    do_while_node->body = body_node;
    push_node(do_while_node);
}

void Parser::parse_include()
{
    if (!next()->isKeyword("include"))
    {
        parse_error("Expecting an include keyword for include statements");
    }

    parse_expression();

    ExpressionInterpretableNode* exp = (ExpressionInterpretableNode*) pop_node();
    IncludeNode* include_node = (IncludeNode*) factory.createNode(NODE_TYPE_INCLUDE);
    include_node->exp = exp;
    push_node(include_node);
}

void Parser::parse_include_once()
{
    if (!next()->isKeyword("include_once"))
    {
        parse_error("Expecting an include_once keyword for include_once statements");
    }

    parse_expression();

    ExpressionInterpretableNode* exp = (ExpressionInterpretableNode*) pop_node();
    IncludeOnceNode* include_once_node = (IncludeOnceNode*) factory.createNode(NODE_TYPE_INCLUDE_ONCE);
    include_once_node->exp = exp;
    push_node(include_once_node);
}

void Parser::parse_permission_node()
{
    if (!next()->isKeyword("permission"))
    {
        parse_error("Expecting a permission keyword for setting scope permissions");
    }

    parse_expression();
    ExpNode* exp = (ExpNode*) pop_node();
    parse_body();
    BodyNode* body = (BodyNode*) pop_node();
    PermissionNode* permission_node = (PermissionNode*) factory.createNode(NODE_TYPE_PERMISSION_NODE);
    permission_node->exp_node = exp;
    permission_node->body_node = body;
    push_node(permission_node);

}
void Parser::parse_multi_expression()
{
    ListNode* list_node = (ListNode*) factory.createNode(NODE_TYPE_LIST);
    if (peek()->isSymbol(";"))
    {
        push_node(list_node);
        return;
    }


    while(1)
    {
        parse_expression_or_variable_declaration();
        list_node->addNode((ExpressionInterpretableNode*) pop_node());
        if (peek()->isSymbol(","))
        {
            next();
            continue;
        }

        break;
    }
    push_node(list_node);
}

void Parser::parse_for()
{
    if (!next()->isKeyword("for"))
    {
        parse_error("Expecting a for keyword for \"for\" statements");
    }

    if (!next()->isSymbol("("))
    {
        parse_error("Expecting a left bracket after the \"for\" keyword");
    }

    // Lets parse the init.
    parse_multi_expression();
    ListNode* init = (ListNode*) pop_node();
    parse_semicolon();

    // Now the condition
    parse_expression();
    ExpressionInterpretableNode* cond = (ExpressionInterpretableNode*) pop_node();
    parse_semicolon();

    // Now the loop
    parse_multi_expression();
    ListNode* loop = (ListNode*) pop_node();

    // Lets get rid of the ")" symbol
    if (!next()->isSymbol(")"))
    {
        parse_error("Expecting a final right bracket before for-loop body. \")\"");
    }
    // And finally the body
    parse_body();
    BodyNode* body = (BodyNode*) pop_node();

    ForNode* for_node = (ForNode*) factory.createNode(NODE_TYPE_FOR);
    for_node->init = init;
    for_node->cond = cond;
    for_node->loop = loop;
    for_node->body = body;
    push_node(for_node);
}

void Parser::parse_break()
{
    if (!next()->isKeyword("break"))
    {
        parse_error("Expecting a break keyword");
    }
    
    BreakNode* break_node = (BreakNode*) factory.createNode(NODE_TYPE_BREAK);
    push_node(break_node);
}

void Parser::parse_continue()
{
    if (!next()->isKeyword("continue"))
    {
        parse_error("Expecting a continue keyword");
    }
    
    ContinueNode* continue_node = (ContinueNode*) factory.createNode(NODE_TYPE_CONTINUE);
    push_node(continue_node);
}

void Parser::parse_semicolon()
{
    Token* token = next();
    if (token == NULL || !token->isSymbol(";"))
    {
        parse_error("Expecting a semicolon but " + (token == NULL ? "nothing" : token->value) + " was provided");
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

void Parser::parse_expression_for_value(int extra_rules)
{
    parse_expression(RULE_PARSE_CASTING | RULE_PARSE_ARRAY | extra_rules);
}


void Parser::parse_expression(int rules)
{
    std::vector<ExpressionInterpretableNode*> nodes;
    std::vector<std::string> operators;
    while(1)
    {
        parse_value(rules);
        ExpressionInterpretableNode* node = (ExpressionInterpretableNode*) pop_node();
        nodes.push_back(node);

        if(!peek()->isOperator())
        {
            break;
        }

        std::string next_op = next()->value;
        if (!operators.empty())
        {
            std::string top_op = operators.back();
            if (first_op_has_priority(top_op, next_op) || top_op == next_op)
            {
                ExpressionInterpretableNode* right_node = nodes.back();
                nodes.pop_back();
                ExpressionInterpretableNode* left_node = nodes.back();
                nodes.pop_back();
                nodes.push_back(factory.createExpNode(left_node, right_node, top_op));
                operators.pop_back();
            }
        }
        operators.push_back(next_op);
    }

    while(!operators.empty())
    {
        std::string top_op = operators.back();
        ExpressionInterpretableNode* right_node = nodes.back();
        nodes.pop_back();
        ExpressionInterpretableNode* left_node = nodes.back();
        nodes.pop_back();
        nodes.push_back(factory.createExpNode(left_node, right_node, top_op));
        operators.pop_back();
    }

    ExpressionInterpretableNode* final_node = nodes.back();
    push_node(final_node);
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
    ExpressionInterpretableNode* next = NULL;
    if (peek()->isKeyword("else"))
    {
       // We have an else or else if statement
       parse_else();
       next = (ExpressionInterpretableNode*) pop_node();
    }
    IfStatementNode* if_stmt = (IfStatementNode*) factory.createNode(NODE_TYPE_IF_STMT);
    if_stmt->exp = exp;
    if_stmt->body = body;
    if_stmt->next = next;
    push_node(if_stmt);
}

void Parser::parse_else()
{
    if (!peek()->isKeyword("else"))
       parse_error("Expecting an else keyword for parsing an else statement");
    // Clear the else keyword
    next();

    // Is this an else if?
    if (peek()->isSymbol("if"))
    {
        // Ok this is an else if lets parse the if statement
        parse_if_stmt();
    }
    else
    {
       // This is an else statement so lets parse the body
       parse_body();
    }
}

void Parser::parse_modifier_access()
{
    access = get_modifier_access_for_string(next()->value);
    if (!next()->isSymbol(":"))
    {
        parse_error("Expecting a \":\" symbol when specifying access");
    }    
}

void Parser::parse_class_body()
{
    if (!next()->isSymbol("{"))
    {
        parse_error("Expecting a left bracket \"{\" for given body");
    }
    
    BodyNode* body_node = (BodyNode*) factory.createNode(NODE_TYPE_BODY);
    while(!peek()->isSymbol("}"))
    {
        ensure_peek_non_dummy("Bodies must be closed with the \"}\" symbol");
        if (this->current_token->isDataAccessKeyword())
        {
            parse_modifier_access();
        }
        else
        {
            parse_class_body_next();
            body_node->addNode((InterpretableNode*) pop_node());
        }
    }
    
    // Lets remove the "}" symbol
    next();
    
    push_node(body_node);
}

void Parser::parse_class_body_next()
{
    parse_body_next();
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
        body_node->addNode((InterpretableNode*) pop_node());
    }
    
    // Lets remove the "}" symbol
    next();
    
    push_node(body_node);
}

void Parser::parse_vafde_for_identifier()
{
    if (!peek()->isIdentifier())
    {
        parse_error("Expecting an identifier for a vafd parse");
    }

     /* The token is an identifier so this can be treated as an expression as it is one of the following
         * 1. Representing a variable
         * 2. An assignment
         * 3. A function call
         * 4. A variable declaration with an object type
         * 5. An expression
         */
         
    Token* next_token = peek(this->current_token, 1);
    if (next_token->isIdentifier() || (next_token->isSymbol("[") && peek(this->current_token, 2)->isSymbol("]") && peek(this->current_token, 3)->isIdentifier()))
    {
        // This is a variable declaration
        parse_variable_declaration();
        return;
    }

    // As this is not a variable declaration it may be a valid expression equaling one of the following specified above.
    parse_expression_for_value();
}

void Parser::parse_expression_or_variable_declaration()
{
    if (peek()->isKeyword() && DataType::isPrimitiveDataType(peek()->getValue()))
    {
        parse_variable_declaration();
        return;
    }

    if (peek()->isIdentifier())
    {
        parse_vafde_for_identifier();
        return;
    }

    parse_error("Expecting either a datatype keyword or an identifier");
}

void Parser::parse_body_next()
{
    std::string token_value = this->current_token->getValue();
    if (this->current_token->isKeyword() && is_datatype(token_value))
    {
        parse_variable_declaration();
        parse_semicolon();
    }
    else if(this->current_token->isSymbol("@"))
    {
        parse_ignore_validation(false);
    }
    else if(this->current_token->isKeyword("if"))
    {
        parse_if_stmt();
    }
    // NEW MAY ALSO BE USED IN EXPRESSIONS; MIGHT NEED TO ALLOW FOR THIS.
    else if(this->current_token->isKeyword("new"))
    {
        parse_new();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("return"))
    {
        parse_return();
        parse_semicolon();   
    }
    else if(this->current_token->isKeyword("function"))
    {
        parse_function();
    }
    else if(this->current_token->isKeyword("final"))
    {
        parse_final();
    }
    else if(this->current_token->isKeyword("pure"))
    {
        // A pure entity is expected
        parse_pure();
    }
    else if(this->current_token->isKeyword("class"))
    {
        parse_class();
    }
    else if(this->current_token->isKeyword("try"))
    {
        parse_try();
    }
    else if(this->current_token->isKeyword("throw"))
    {
        parse_throw();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("while"))
    {
        parse_while();
    }
    else if(this->current_token->isKeyword("do"))
    {
        parse_do_while();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("for"))
    {
        parse_for();
    }
    else if(this->current_token->isKeyword("break"))
    {
        parse_break();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("continue"))
    {
        parse_continue();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("include"))
    {
        parse_include();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("include_once"))
    {
        parse_include_once();
        parse_semicolon();
    }
    else if(this->current_token->isKeyword("permission"))
    {
        parse_permission_node();
    }
    else if(this->current_token->isIdentifier())
    {
        parse_vafde_for_identifier();
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
    this->prev_token = NULL;
    this->root_node = NULL;
    this->current_node = NULL;
    this->current_token = root_token;
    while(this->current_token != NULL)
    {
        global_parse_next();
    }

    return this->root_node;
}

Logger* Parser::getLogger()
{
    return this->logger;
}

