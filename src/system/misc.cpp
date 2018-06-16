#include "misc.h"
#include "config.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <stdlib.h>
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


bool startsWith(std::string mainStr, std::string toMatch)
{
	// std::string::find returns 0 if toMatch is found at starting
	if(mainStr.find(toMatch) == 0)
		return true;
	else
		return false;
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
		throw;
    }
}

std::vector<std::string> str_split(std::string str, std::string delm)
{
	std::vector<std::string> vec;
	std::size_t pos = 0;
	std::size_t old_pos = 0;
	while((pos = str.find(delm, old_pos)) != std::string::npos)
	{
		vec.push_back(str.substr(old_pos, (pos-old_pos)));
		old_pos = pos+delm.size();
	}
	
	// Store the final piece of the puzzle
	vec.push_back(str.substr(old_pos, (str.size()-old_pos)));
	return vec;
}


std::string writeTemp(const char* data, int len)
{
   time_t t;

   /* Intializes random number generator */
   srand((unsigned) time(&t));

   int rand_num = rand();
   std::string random_filename = std::to_string(rand_num);
   std::string path = std::string(TMP_DIRECTORY) + "/" + random_filename + ".dat";
   FILE* f = fopen(path.c_str(), "w");
   if (f == NULL)
	 return "FAILED_OPEN";
   if(fwrite(data, len, 1, f) != 1)
	 return "FAILED_WRITE";
	 
   fclose(f);

   return path;
}