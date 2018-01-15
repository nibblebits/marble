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
    ModuleSystem moduleSystem;
    Interpreter interpreter(moduleSystem.getClassSystem(), moduleSystem.getFunctionSystem());
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    
    Logger* logger = interpreter.getLogger();
    interpreter.setModuleSystem(&moduleSystem);
    moduleSystem.loadModule("./mods/marble_iomod.so");
    interpreter.runScript("./test.marble");

    if (moduleSystem.getClassSystem()->hasClassWithName("Exception"))
        throw std::logic_error("Oh shit..");

    for (LogEntry entry : logger->entries)
    {
        std::cout << entry.message << " on line: " << entry.posInfo.line << ", col: " << entry.posInfo.col << std::endl;
    }    
    std::cout << "END OF SCOPE" << std::endl;
}
int main(int argc, char** argv)
{
    interpret();
    std::cout << "Program terminated" << std::endl;
        std::cout << "ALLOCATIONS: " << allocations.size() << std::endl;
    if (allocations.size() > 0)
    {
        std::cout << "MEMORY LEAK DETECTED" << std::endl;
        // We won't let the leak continue.
        for(void* ptr : allocations)
            free(ptr);
    }
    return 0;
}
