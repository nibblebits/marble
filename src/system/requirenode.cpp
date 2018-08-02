#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
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
        this->splitter = interpreter->loadScript(this->filename.c_str());
        split split;
        PosInfo posInfo;
        posInfo.filename = this->filename.c_str();
        posInfo.line = 1;
        posInfo.col = 1;
        
        // Merge the splits to create one final code result
        while(this->splitter.split(&split))
        {
            // Merge the code and data for this split
            this->code_result += interpreter->mergeCodeAndDataForSplit(&split);
        }

        Node* root_node = interpreter->getAST(code_result.c_str(), posInfo);
        validator->validate(root_node);
        Logger* logger = validator->getLogger();
        if (logger->hasErrors())
        {
            throw TestError(logger->entries[0].message);
        }
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

    PosInfo posInfo;
    posInfo.filename = this->filename.c_str();
    posInfo.line = 1;
    posInfo.col = 1;
    // Now we are done let's run the result we also want to ignore validation as we did it during testing
    interpreter->run(this->code_result.c_str(), posInfo, true);
    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}