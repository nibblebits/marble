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