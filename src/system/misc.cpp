#include "misc.h"
#include "config.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <linux/limits.h>
#include <sys/mman.h>

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
   srand((unsigned) time(&t) + len);

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

std::string str_replace(std::string data, std::string to_replace, std::string replace_with)
{
	size_t current_pos = 0;
	while((current_pos = data.find(to_replace, current_pos)) != std::string::npos)
	{
		data = data.replace(current_pos, to_replace.size(), replace_with);
		current_pos += replace_with.size();
	}

	return data;
}

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, 0, 0);
}