#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "interpreter.h"

void interpret()
{
    Interpreter interpreter;
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    try
    {
        interpreter.runScript("./test.marble");
    } 
    catch(...)
    {
        std::cout << "Exception was thrown" << std::endl;
    }
    
    Logger* logger = interpreter.getLogger();
    for (LogEntry entry : logger->entries)
    {
        std::cout << entry.message << " line: " << entry.posInfo.line << ", col: " << entry.posInfo.col << std::endl;
    }    
    std::cout << "END OF SCOPE" << std::endl;
}
int main(int argc, char** argv)
{
    interpret();
    std::cout << "Program terminated" << std::endl;
    return 0;
}
