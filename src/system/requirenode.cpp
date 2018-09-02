#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
#include "filepermission.h"
#include "misc.h"
#include "exceptions/testerror.h"
#include "exceptions/IOException.h"

RequireNode::RequireNode() : InterpretableNode(NODE_TYPE_REQUIRE)
{
    this->required_ast = NULL;
}

RequireNode::~RequireNode()
{
    this->splitter.free();
}


void RequireNode::test(Validator* validator, struct extras extra)
{
    this->code_result = "";
    try
    {
        Interpreter* interpreter = validator->getInterpreter();
        std::string absolute_path = getAbsolutePath(this->filename);
        this->splitter = interpreter->loadScript(absolute_path.c_str());
        split split;
        PosInfo posInfo;
        posInfo.filename = absolute_path;
        posInfo.line = 1;
        posInfo.col = 1;
        
        // Merge the splits to create one final code result
        while(this->splitter.split(&split))
        {
            // Merge the code and data for this split
            this->code_result += interpreter->mergeCodeAndDataForSplit(&split);
        }
        Node* root_node = interpreter->getAST(code_result.c_str(), posInfo);
        this->required_ast = (InterpretableNode*) root_node;
        
        // Test the AST
        validator->validate(this->required_ast);
    }
    catch(TestError& ex)
    {
        throw TestError("Failed to require file: " + this->filename + " " + ex.what());
    }
    catch(IOException& ex)
    {
        throw TestError("There was a problem requiring the file: " + this->filename + ", message: " + ex.what());
    }
  /*  catch(...)
    {
        throw TestError("There was a problem requiring the file: " + this->filename + " we do not know why");
    }*/

}

Value RequireNode::interpret(Interpreter* interpreter, struct extras extra)
{

    std::string absolute_path = getAbsolutePath(this->filename);
    // Before we do anything let's make sure we have permission to load this file
    FilePermission::checkPermissionAllows(interpreter, interpreter->getCurrentScope(), absolute_path.c_str(), "r");

    // Run the nodes for the file that was required
    InterpretableNode* current_node = this->required_ast;
    while(current_node != NULL)
    {
        current_node->interpret(interpreter);
        current_node = (InterpretableNode*) current_node->next;
    }

    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}