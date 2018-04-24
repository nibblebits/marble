#include "misc.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <cstring>
#include <stdexcept>
#include <linux/limits.h>
std::string getAbsolutePath(std::string uri)
{
    char resolved_path[PATH_MAX];
    realpath(uri.c_str(), resolved_path);
    return std::string(resolved_path);
}

std::string urlDecode(std::string &eString) {
	std::string ret;
	char ch;
	int i, j;
	for (i=0; i<eString.length(); i++) {
		if (int(eString[i])==37) {
			sscanf(eString.substr(i+1,2).c_str(), "%x", &j);
			ch=static_cast<char>(j);
			ret+=ch;
			i=i+2;
		} else {
			ret+=eString[i];
		}
	}
	return (ret);
}

void CloneForCall(const void* ptr, int size, int new_size, std::function<void(const void*, int)> func)
{
	if (size > new_size)
		throw std::logic_error("Your new size must be equal or greater than size. Truncation is not supported");
	char* cloned_data = new char[new_size];
    memcpy(cloned_data, ptr, size);
    try
    {
		func((const void*) cloned_data, new_size);
        delete cloned_data;
    }
    catch(...)
    {
        delete cloned_data;
    }
}