#include "misc.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <linux/limits.h>
std::string getAbsolutePath(std::string uri)
{
    char resolved_path[PATH_MAX];
    realpath(uri.c_str(), resolved_path);
    return std::string(resolved_path);
}