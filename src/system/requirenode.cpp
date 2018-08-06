#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
#include "filepermission.h"
#include "misc.h"
#include "exceptions/testerror.h"
#include "exceptions/IOException.h"

RequireNode::RequireNode() : InterpretableNode(NODE_TYPE_REQUIRE)
{

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
        Node* final_node = root_node;
        while(1)
        {
            if (final_node->next == NULL)
                break;
            final_node = final_node->next;
        }

        Node* after_node = validator->current_node->next;
        // Replace this require node with the new AST. The previous nodes next is us
        if (validator->previous_node == NULL)
        {
            validator->current_node->next = (InterpretableNode*) root_node;
        }
        else
        {
            validator->previous_node->next = (InterpretableNode*) root_node;
            validator->current_node = validator->previous_node;
        }
        final_node->next = after_node;
    }
    catch(TestError& ex)
    {
        throw TestError("Failed to require file: " + this->filename + " " + ex.what());
    }
    catch(IOException& ex)
    {
        throw TestError("There was a problem requiring the file: " + this->filename + ", message: " + ex.what());
    }
    catch(...)
    {
        throw TestError("There was a problem requiring the file: " + this->filename + " we do not know why");
    }

}

Value RequireNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v2;
    return v2;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}