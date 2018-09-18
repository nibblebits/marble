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

#ifndef CONFIG_H
#define CONFIG_H


// Marble versioning information
#define MARBLE_MAJOR_CODENAME "Clearies"
#define MARBLE_VERSION "0.1.1"

#define MAX_KEYWORD_SIZE 15
#define MAX_OPERATORS_SIZE 3

#define MAX_OUTPUTSTREAM_SIZE_UNTIL_AUTO_FLUSH 1024

#define TMP_DIRECTORY "/tmp"

// Debug mode is enabled; To display comment the line below
#define DEBUG_ENABLED

#endif
