#ifndef MISC_H
#define MISC_H
#include <string>
#include <vector>
#include <functional>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


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

/**
 * 
 * Returns true if the given filename exists otherwise false
 */

bool file_exists(std::string filename);

/**
 * Converts the integer to a hex value
 */

std::string to_hex(int c);


/**
 * Returns a random hex string that is the size of total_bytes
 */
std::string random_hex(size_t total_bytes);

/**
 * Returns the provided string in lower case
 */
std::string to_lower(std::string s);


/**
 * Returns true if the given path is a file
 */
bool isFile(std::string path);

/**
 * Matches all occurences in the target provided based on the regex provided.
 * \return Returns a vector of string that holds the occurences found
 */
std::vector<std::string> preg_match_all(std::string target, std::string regex);

/**
 * Trims all whitespace on the left of this string
 */
std::string trim_left(const std::string& str, std::string pattern="\f\n\r\t\v");
/**
 * Trims all whitespace on the right of this string
 */
std::string trim_right(const std::string& str, std::string pattern="\f\n\r\t\v");
/**
 * Trims all whitespace on the left and right of this string
 */
std::string trim(const std::string& str, std::string pattern="\f\n\r\t\v");



#endif