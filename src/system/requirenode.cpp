#include "requirenode.h"
#include "interpreter.h"
#include "validator.h"
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
            #warning Improve this
            char* code_data = new char[split.code.size+1];
            memcpy(code_data, split.code.data, split.code.size);
            code_data[split.code.size] = 0;
            // Marble tag "<marble>" should be added to the current position
            posInfo.col += strlen(MARBLE_OPEN_TAG);
            Node* root_node = interpreter->getAST(code_data, posInfo);
            validator->validate(root_node);
            delete code_data;
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
        if (split.has_data)
        {
            char* output_data = new char[split.output.size+1];
            memcpy(output_data, split.output.data, split.output.size);
            output_data[split.output.size] = 0;
            interpreter->output(output_data);
            delete output_data;
        }

        // Run the code
        if (split.has_code)
        {
            char* code_data = new char[split.code.size+1];
            memcpy(code_data, split.code.data, split.code.size);
            code_data[split.code.size] = 0;
            // Marble tag "<marble>" should be added to the current position
            posInfo.col += strlen(MARBLE_OPEN_TAG);
            interpreter->run(code_data, posInfo, true);
            delete code_data;
        }
        this->splits.pop_front();
    }

    Value v;
    return v;
}

void RequireNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}