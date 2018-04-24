#ifndef MISC_H
#define MISC_H
#include <string>
#include <functional>

std::string getAbsolutePath(std::string uri);
std::string urlDecode(std::string &eString);

/**
 * Clones the provided pointer, calls the given function then frees the cloned pointer
 * 
 * \param ptr The data to clone
 * \param size The size of the data to clone
 * \param new_size The new size to create
 * \param func The function to call to pass this cloned data to
 */
void CloneForCall(const void* ptr, int size, int new_size, std::function<void(const void*, int)> func);
#endif