#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "interpreter.h"

std::vector<void*> allocations;
bool wait = false;
int size = 0;
void* operator new(size_t size)
{
    void* allocation = malloc(size);
    if (!wait) {
        wait = true;
        allocations.push_back(allocation);
        wait = false;
    }
    return allocation;
}

void operator delete(void* ptr)
{
    if (!wait) {
        wait = true;
        allocations.erase(std::remove(allocations.begin(), allocations.end(), ptr), allocations.end());
        wait = false;
    }
    free(ptr);
}

void operator delete[](void* ptr)
{
    allocations.erase(std::remove(allocations.begin(), allocations.end(), ptr), allocations.end());
    free(ptr);
}
void interpret()
{
    Interpreter interpreter;
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    
    interpreter.runScript("./test.marble");    
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
        std::cout << "ALLOCATIONS: " << allocations.size() << std::endl;
    return 0;
}
