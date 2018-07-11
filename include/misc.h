#ifndef MISC_H
#define MISC_H
#include <string>
#include <vector>
#include <functional>

std::string getAbsolutePath(std::string uri);
std::string urlDecode(std::string &eString);

bool startsWith(std::string mainStr, std::string toMatch);

/**
 * Clones the provided pointer, calls the given function then frees the cloned pointer
 * 
 * \param ptr The data to clone
 * \param size The size of the data to clone
 * \param new_size The new size to create
 * \param func The function to call to pass this cloned data to
 */
void CloneForCall(const void* ptr, int size, int new_size, std::function<void(const void*, int)> func);


std::vector<std::string> str_split(std::string str, std::string delm);

/**
 *  Writes the data as a file to the temporary directory in the system then returns the path
*/
std::string writeTemp(const char* data, int len);

/**
 * Replaces to_replace with replace_with for the given data
 */
std::string str_replace(std::string data, std::string to_replace, std::string replace_with);

/**
 * Creates memory that can be shared between child processes created from a fork
 */
void* create_shared_memory(size_t size);
#endif