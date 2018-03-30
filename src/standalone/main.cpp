#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <memory>
#include "interpreter.h"
#include "basesystemhandler.h"
#include "object.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "../stdmods/iomod/include/iopermission.h"

/*std::vector<void*> allocations;
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
*/
void interpret()
{
    BaseSystemHandler* baseHandler = new BaseSystemHandler();
    ModuleSystem* moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem());
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    
    interpreter.setInputFunction([]()->std::string {
        std::string s;
        std::cin >> s;
        return s;
    });

    Logger* logger = interpreter.getLogger();
    moduleSystem->loadModule("./mods/marble_filemod.so");
    moduleSystem->loadModule("./mods/marble_iomod.so");
    interpreter.setModuleSystem(moduleSystem);
    // Let's just add the IOPermission just for now rather than rely on a configuration file
    std::shared_ptr<PermissionObject> permission = std::dynamic_pointer_cast<PermissionObject>(Object::create(moduleSystem->getClassSystem()->getClassByName("IOPermission")));
    interpreter.getCurrentScope()->permissions->addPermission(permission);
    interpreter.runScript("./test.marble");

    for (LogEntry entry : logger->entries)
    {
        std::cout << entry.message << " on line: " << entry.posInfo.line << ", col: " << entry.posInfo.col << std::endl;
    }    
}
int main(int argc, char** argv)
{
  
        interpret();
  
    std::cout << "Program terminated" << std::endl;
    /*
        std::cout << "ALLOCATIONS: " << allocations.size() << std::endl;
    if (allocations.size() > 0)
    {
        std::cout << "MEMORY LEAK DETECTED" << std::endl;
        // We won't let the leak continue.
        for(void* ptr : allocations)
            free(ptr);
    }
    */
    return 0;
}
