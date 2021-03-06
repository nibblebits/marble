/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "misc.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <linux/limits.h>
#include <sys/mman.h>

std::string getAbsolutePath(std::string uri)
{
	char resolved_path[PATH_MAX];
	realpath(uri.c_str(), resolved_path);
	return std::string(resolved_path);
}

std::string urlDecode(std::string &eString)
{
	std::string ret;
	char ch;
	int i, j;
	for (i = 0; i < eString.length(); i++)
	{
		if (int(eString[i]) == 37)
		{
			sscanf(eString.substr(i + 1, 2).c_str(), "%x", &j);
			ch = static_cast<char>(j);
			ret += ch;
			i = i + 2;
		}
		else
		{
			ret += eString[i];
		}
	}
	return (ret);
}

bool startsWith(std::string mainStr, std::string toMatch)
{
	// std::string::find returns 0 if toMatch is found at starting
	if (mainStr.find(toMatch) == 0)
		return true;
	else
		return false;
}

void CloneForCall(const void *ptr, int size, int new_size, std::function<void(const void *, int)> func)
{
	if (size > new_size)
		throw std::logic_error("Your new size must be equal or greater than size. Truncation is not supported");
	char *cloned_data = new char[new_size];
	memcpy(cloned_data, ptr, size);
	try
	{
		func((const void *)cloned_data, new_size);
		delete[] cloned_data;
	}
	catch (...)
	{
		delete[] cloned_data;
		throw;
	}
}

std::vector<std::string> str_split(std::string str, std::string delm)
{
	std::vector<std::string> vec;
	std::size_t pos = 0;
	std::size_t old_pos = 0;
	while ((pos = str.find(delm, old_pos)) != std::string::npos)
	{
		vec.push_back(str.substr(old_pos, (pos - old_pos)));
		old_pos = pos + delm.size();
	}

	// Store the final piece of the puzzle
	vec.push_back(str.substr(old_pos, (str.size() - old_pos)));
	return vec;
}

std::string writeTemp(const char *data, int len)
{
	time_t t;

	/* Intializes random number generator */
	srand((unsigned)time(&t) + len);

	int rand_num = rand();
	std::string random_filename = std::to_string(rand_num);
	std::string path = std::string(TMP_DIRECTORY) + "/" + random_filename + ".dat";

	FILE *f = fopen(path.c_str(), "w");
	if (f == NULL)
		return "FAILED_OPEN";
	if (fwrite(data, len, 1, f) != 1)
		return "FAILED_WRITE";

	fclose(f);

	return path;
}

std::string str_replace(std::string data, std::string to_replace, std::string replace_with)
{
	size_t current_pos = 0;
	if (to_replace == "" || data == "")
		return data;

	while ((current_pos = data.find(to_replace, current_pos)) != std::string::npos)
	{
		data = data.replace(current_pos, to_replace.size(), replace_with);
		current_pos += replace_with.size();
	}

	return data;
}

void *create_shared_memory(size_t size)
{
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

bool file_exists(std::string filename)
{
	std::ifstream ifile(filename);
	return ifile.good();
}

std::string to_hex(int c)
{
	std::stringstream ss;
	ss << std::hex << c;
	return ss.str();
}

std::string random_hex(size_t total_bytes)
{
	char binary_buf[total_bytes];
	// Let's open /dev/urandom
	FILE *file = fopen("/dev/urandom", "r");
	if (!file)
		throw std::logic_error("Failed to generate random hex as failed to open /dev/urandom");

	if (fread(binary_buf, total_bytes, 1, file) == 0)
		throw std::logic_error("Failed to read from /dev/urandom");

	fclose(file);

	std::string result = "";
	for (int i = 0; i < total_bytes; i++)
	{
		result += to_hex(binary_buf[i]);
	}

	return result.substr(0, total_bytes);
}

std::string to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

bool isFile(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::vector<std::string> preg_match_all(std::string target, std::string regex)
{
	std::vector<std::string> occurrences;
	std::string s(target);
	std::smatch m;
	std::regex e(regex);
	while (std::regex_search(s, m, e))
	{
		for (auto x : m)
		{
			occurrences.push_back(x.str());
		}
		s = m.suffix().str();
	}

	return occurrences;
}

std::string trim_left(const std::string& str, std::string pattern)
{
  return str.substr(str.find_first_not_of(pattern));
}


std::string trim_right(const std::string& str, std::string pattern)
{
  return str.substr(0,str.find_last_not_of(pattern) + 1);
}

std::string trim(const std::string& str, std::string pattern)
{
  return trim_left(trim_right(str, pattern), pattern);
}

std::string number_format(std::string target, int places)
{
	std::vector<std::string> splits = str_split(target, ".");

	if (places == 0)
		return splits[0];
	
	std::string second_split = (splits.size() > 1 ? splits[1] : "");
	std::string to_add = "";
	int total_to_add = places - second_split.size();
	if (total_to_add > 0)
	{
		for (int i = 0; i < total_to_add; i++)
			to_add += "0";
	}
	else
	{
		second_split = second_split.substr(0, places);
	}
	return splits[0] + "." + second_split + to_add;
}

std::string exec(std::string cmd, int& return_value) {
    std::array<char, 128> buffer;
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }
    
    return_value = fclose(pipe) / 256;
    return result;
}


std::string get_current_timezone()
{
	time_t t = time(NULL);
	struct tm lt = {0};

	localtime_r(&t, &lt);

	return lt.tm_zone;
}