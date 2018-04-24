#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
#include "misc.h"

RequireNode::RequireNode() : InterpretableNode(NODE_TYPE_REQUIRE)
{

}

RequireNode::~RequireNode()
{
    this->splitter.free();
}

void RequireNode::test(Validator* validator, struct extras extra)
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
                interpreter->handleLineAndColumn(&posInfo, code_data, size);
            });
        }
        this->splits.push_back(split);
    }

}

Value RequireNode::interpret(Interpreter* interpreter, struct extras extra)
{

    PosInfo posInfo;
    posInfo.filename = this->filename.c_str();
    posInfo.line = 1;
    posInfo.col = 1;
    while(!this->splits.empty())
    {
        split split = this->splits.front();
        // Output the data
        if (split.has_data)
        {
            posInfo = interpreter->handleRawDataForSplit(posInfo, &split);
        }

        // Run the code
        if (split.has_code)
        {
            posInfo = interpreter->handleCodeDataForSplit(posInfo, &split, true);
        }

        this->splits.pop_front();
    }

    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}