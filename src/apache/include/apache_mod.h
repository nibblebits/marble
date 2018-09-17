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

#ifndef APACHEMOD_H
#define APACHEMOD_H

extern "C"
{
    #include "httpd.h"
    #include "http_config.h"
    #include "http_core.h"
    #include "http_protocol.h"
    #include "http_request.h"
    #include "ap_config.h"

    #include "apr_strings.h"
    #include "apr_network_io.h"
    #include "apr_md5.h"
    #include "apr_sha1.h"
    #include "apr_hash.h"
    #include "apr_base64.h"
    #include "apr_dbd.h"
    #include <apr_file_info.h>
    #include <apr_file_io.h>
    #include <apr_tables.h>
}

typedef struct {
    const char *key;
    const char *value;
} keyValuePair;

keyValuePair *readPost(request_rec *r);
int util_read(request_rec *r, const char **rbuf, apr_off_t *size);
#endif