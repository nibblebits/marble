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
    try
    {
        Interpreter* interpreter = validator->getInterpreter();
        this->splitter = interpreter->loadScript(this->filename.c_str());
        split split;
        PosInfo posInfo;
        posInfo.filename = this->filename.c_str();
        posInfo.line = 1;
        posInfo.col = 1;
        while(this->splitter.split(&split))
        {
            if (split.has_code)
            {
                CloneForCall(split.code.data, split.code.size, split.code.size+1, [&](const void* ptr, int size) {
                    char* code_data = (char*) ptr;
                    code_data[split.code.size] = 0;
                    // Marble tag "<marble>" should be added to the current position
                    posInfo.col += strlen(MARBLE_OPEN_TAG);
                    Node* root_node = interpreter->getAST(code_data, posInfo);
                    validator->validate(root_node);
                    Logger* logger = validator->getLogger();
                    if (logger->hasErrors())
                    {
                       throw TestError(logger->entries[0].message);
                    }
                    interpreter->handleLineAndColumn(posInfo, code_data, size);
                });
            }
            this->splits.push_back(split);
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
    std::string result = "";

    while(!this->splits.empty())
    {        
        split split = this->splits.front();
        result += interpreter->mergeCodeAndDataForSplit(&split);
        this->splits.pop_front();
    }

    // Now we are done let's run the result we also want to ignore validation as we did it during testing
    interpreter->run(result.c_str(), posInfo, true);
    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}