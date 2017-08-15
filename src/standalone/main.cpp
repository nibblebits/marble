#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "interpreter.h"

void interpret()
{
    Interpreter interpreter;
    interpreter.runScript("./test.marble");  
}
int main(int argc, char** argv) 
{
   interpret();
   std::cout << "Program terminated" << std::endl; 
   return 0; 
}
