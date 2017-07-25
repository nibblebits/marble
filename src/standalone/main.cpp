#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "interpreter.h"

int main(int argc, char** argv) 
{
    Interpreter interpreter;
    interpreter.runScript("./test.marble");   
}
