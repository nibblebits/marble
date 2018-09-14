#include "../include/curlobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "networkmod.h"
#include "networkpermission.h"
#include "misc.h"
#include "array.h"

CurlObject::CurlObject(Class *c) : Object(c)
{
    this->curl = NULL;
}

CurlObject::~CurlObject()
{
    // Clean up if the programmer failed to
    if (this->curl != NULL)
        curl_easy_cleanup(this->curl);

    if (!this->lists_to_free.empty())
    {
        for (struct curl_slist *slist : this->lists_to_free)
            curl_slist_free_all(slist);
    }
}

std::shared_ptr<Object> CurlObject::newInstance(Class *c)
{
    return std::make_shared<CurlObject>(c);
}

void CurlObject::newInterpreter(Interpreter *interpreter)
{

    // Let's create CURL variables for curl
    Scope *scope = interpreter->getRootScope();
    scope->createVariable("CURLOPT_AUTOREFERER", 58);
    scope->createVariable("CURLOPT_BINARYTRANSFER", 19914);
    scope->createVariable("CURLOPT_BUFFERSIZE", 98);
    scope->createVariable("CURLOPT_CAINFO", 10065);
    scope->createVariable("CURLOPT_CAPATH", 10097);
    scope->createVariable("CURLOPT_CONNECTTIMEOUT", 78);
    scope->createVariable("CURLOPT_COOKIE", 10022);
    scope->createVariable("CURLOPT_COOKIEFILE", 10031);
    scope->createVariable("CURLOPT_COOKIEJAR", 10082);
    scope->createVariable("CURLOPT_COOKIESESSION", 96);
    scope->createVariable("CURLOPT_CRLF", 27);
    scope->createVariable("CURLOPT_CUSTOMREQUEST", 10036);
    scope->createVariable("CURLOPT_DNS_CACHE_TIMEOUT", 92);
    scope->createVariable("CURLOPT_DNS_USE_GLOBAL_CACHE", 91);
    scope->createVariable("CURLOPT_EGDSOCKET", 10077);
    scope->createVariable("CURLOPT_ENCODING", 10102);
    scope->createVariable("CURLOPT_FAILONERROR", 45);
    scope->createVariable("CURLOPT_FILE", 10001);
    scope->createVariable("CURLOPT_FILETIME", 69);
    scope->createVariable("CURLOPT_FOLLOWLOCATION", 52);
    scope->createVariable("CURLOPT_FORBID_REUSE", 75);
    scope->createVariable("CURLOPT_FRESH_CONNECT", 74);
    scope->createVariable("CURLOPT_FTPAPPEND", 50);
    scope->createVariable("CURLOPT_FTPLISTONLY", 48);
    scope->createVariable("CURLOPT_FTPPORT", 10017);
    scope->createVariable("CURLOPT_FTP_USE_EPRT", 106);
    scope->createVariable("CURLOPT_FTP_USE_EPSV", 85);
    scope->createVariable("CURLOPT_HEADER", 42);
    scope->createVariable("CURLOPT_HEADERFUNCTION", 20079);
    scope->createVariable("CURLOPT_HTTP200ALIASES", 10104);
    scope->createVariable("CURLOPT_HTTPGET", 80);
    scope->createVariable("CURLOPT_HTTPHEADER", 10023);
    scope->createVariable("CURLOPT_HTTPPROXYTUNNEL", 61);
    scope->createVariable("CURLOPT_HTTP_VERSION", 84);
    scope->createVariable("CURLOPT_INFILE", 10009);
    scope->createVariable("CURLOPT_INFILESIZE", 14);
    scope->createVariable("CURLOPT_INTERFACE", 10062);
    scope->createVariable("CURLOPT_KRB4LEVEL", 10063);
    scope->createVariable("CURLOPT_LOW_SPEED_LIMIT", 19);
    scope->createVariable("CURLOPT_LOW_SPEED_TIME", 20);
    scope->createVariable("CURLOPT_MAXCONNECTS", 71);
    scope->createVariable("CURLOPT_MAXREDIRS", 68);
    scope->createVariable("CURLOPT_NETRC", 51);
    scope->createVariable("CURLOPT_NOBODY", 44);
    scope->createVariable("CURLOPT_NOPROGRESS", 43);
    scope->createVariable("CURLOPT_NOSIGNAL", 99);
    scope->createVariable("CURLOPT_PORT", 3);
    scope->createVariable("CURLOPT_POST", 47);
    scope->createVariable("CURLOPT_POSTFIELDS", 10015);
    scope->createVariable("CURLOPT_POSTQUOTE", 10039);
    scope->createVariable("CURLOPT_PREQUOTE", 10093);
    scope->createVariable("CURLOPT_PRIVATE", 10103);
    scope->createVariable("CURLOPT_PROGRESSFUNCTION", 20056);
    scope->createVariable("CURLOPT_PROXY", 10004);
    scope->createVariable("CURLOPT_PROXYPORT", 59);
    scope->createVariable("CURLOPT_PROXYTYPE", 101);
    scope->createVariable("CURLOPT_PROXYUSERPWD", 10006);
    scope->createVariable("CURLOPT_PUT", 54);
    scope->createVariable("CURLOPT_QUOTE", 10028);
    scope->createVariable("CURLOPT_RANDOM_FILE", 10076);
    scope->createVariable("CURLOPT_RANGE", 10007);
    scope->createVariable("CURLOPT_READDATA", 10009);
    scope->createVariable("CURLOPT_READFUNCTION", 20012);
    scope->createVariable("CURLOPT_REFERER", 10016);
    scope->createVariable("CURLOPT_RESUME_FROM", 21);
    scope->createVariable("CURLOPT_RETURNTRANSFER", 19913);
    scope->createVariable("CURLOPT_SHARE", 10100);
    scope->createVariable("CURLOPT_SSLCERT", 10025);
    scope->createVariable("CURLOPT_SSLCERTPASSWD", 10026);
    scope->createVariable("CURLOPT_SSLCERTTYPE", 10086);
    scope->createVariable("CURLOPT_SSLENGINE", 10089);
    scope->createVariable("CURLOPT_SSLENGINE_DEFAULT", 90);
    scope->createVariable("CURLOPT_SSLKEY", 10087);
    scope->createVariable("CURLOPT_SSLKEYPASSWD", 10026);
    scope->createVariable("CURLOPT_SSLKEYTYPE", 10088);
    scope->createVariable("CURLOPT_SSLVERSION", 32);
    scope->createVariable("CURLOPT_SSL_CIPHER_LIST", 10083);
    scope->createVariable("CURLOPT_SSL_VERIFYHOST", 81);
    scope->createVariable("CURLOPT_SSL_VERIFYPEER", 64);
    scope->createVariable("CURLOPT_STDERR", 10037);
    scope->createVariable("CURLOPT_TELNETOPTIONS", 10070);
    scope->createVariable("CURLOPT_TIMECONDITION", 33);
    scope->createVariable("CURLOPT_TIMEOUT", 13);
    scope->createVariable("CURLOPT_TIMEVALUE", 34);
    scope->createVariable("CURLOPT_TRANSFERTEXT", 53);
    scope->createVariable("CURLOPT_UNRESTRICTED_AUTH", 105);
    scope->createVariable("CURLOPT_UPLOAD", 46);
    scope->createVariable("CURLOPT_URL", 10002);
    scope->createVariable("CURLOPT_USERAGENT", 10018);
    scope->createVariable("CURLOPT_USERPWD", 10005);
    scope->createVariable("CURLOPT_VERBOSE", 41);
    scope->createVariable("CURLOPT_WRITEFUNCTION", 20011);
    scope->createVariable("CURLOPT_WRITEHEADER", 10029);
    scope->createVariable("CURLE_ABORTED_BY_CALLBACK", 42);
    scope->createVariable("CURLE_BAD_CALLING_ORDER", 44);
    scope->createVariable("CURLE_BAD_CONTENT_ENCODING", 61);
    scope->createVariable("CURLE_BAD_DOWNLOAD_RESUME", 36);
    scope->createVariable("CURLE_BAD_FUNCTION_ARGUMENT", 43);
    scope->createVariable("CURLE_BAD_PASSWORD_ENTERED", 46);
    scope->createVariable("CURLE_COULDNT_CONNECT", 7);
    scope->createVariable("CURLE_COULDNT_RESOLVE_HOST", 6);
    scope->createVariable("CURLE_COULDNT_RESOLVE_PROXY", 5);
    scope->createVariable("CURLE_FAILED_INIT", 2);
    scope->createVariable("CURLE_FILE_COULDNT_READ_FILE", 37);
    scope->createVariable("CURLE_FTP_ACCESS_DENIED", 9);
    scope->createVariable("CURLE_FTP_BAD_DOWNLOAD_RESUME", 36);
    scope->createVariable("CURLE_FTP_CANT_GET_HOST", 15);
    scope->createVariable("CURLE_FTP_CANT_RECONNECT", 16);
    scope->createVariable("CURLE_FTP_COULDNT_GET_SIZE", 32);
    scope->createVariable("CURLE_FTP_COULDNT_RETR_FILE", 19);
    scope->createVariable("CURLE_FTP_COULDNT_SET_ASCII", 29);
    scope->createVariable("CURLE_FTP_COULDNT_SET_BINARY", 17);
    scope->createVariable("CURLE_FTP_COULDNT_STOR_FILE", 25);
    scope->createVariable("CURLE_FTP_COULDNT_USE_REST", 31);
    scope->createVariable("CURLE_FTP_PARTIAL_FILE", 18);
    scope->createVariable("CURLE_FTP_PORT_FAILED", 30);
    scope->createVariable("CURLE_FTP_QUOTE_ERROR", 21);
    scope->createVariable("CURLE_FTP_USER_PASSWORD_INCORRECT", 10);
    scope->createVariable("CURLE_FTP_WEIRD_227_FORMAT", 14);
    scope->createVariable("CURLE_FTP_WEIRD_PASS_REPLY", 11);
    scope->createVariable("CURLE_FTP_WEIRD_PASV_REPLY", 13);
    scope->createVariable("CURLE_FTP_WEIRD_SERVER_REPLY", 8);
    scope->createVariable("CURLE_FTP_WEIRD_USER_REPLY", 12);
    scope->createVariable("CURLE_FTP_WRITE_ERROR", 20);
    scope->createVariable("CURLE_FUNCTION_NOT_FOUND", 41);
    scope->createVariable("CURLE_GOT_NOTHING", 52);
    scope->createVariable("CURLE_HTTP_NOT_FOUND", 22);
    scope->createVariable("CURLE_HTTP_PORT_FAILED", 45);
    scope->createVariable("CURLE_HTTP_POST_ERROR", 34);
    scope->createVariable("CURLE_HTTP_RANGE_ERROR", 33);
    scope->createVariable("CURLE_HTTP_RETURNED_ERROR", 22);
    scope->createVariable("CURLE_LDAP_CANNOT_BIND", 38);
    scope->createVariable("CURLE_LDAP_SEARCH_FAILED", 39);
    scope->createVariable("CURLE_LIBRARY_NOT_FOUND", 40);
    scope->createVariable("CURLE_MALFORMAT_USER", 24);
    scope->createVariable("CURLE_OBSOLETE", 50);
    scope->createVariable("CURLE_OK", 0);
    scope->createVariable("CURLE_OPERATION_TIMEDOUT", 28);
    scope->createVariable("CURLE_OPERATION_TIMEOUTED", 28);
    scope->createVariable("CURLE_OUT_OF_MEMORY", 27);
    scope->createVariable("CURLE_PARTIAL_FILE", 18);
    scope->createVariable("CURLE_READ_ERROR", 26);
    scope->createVariable("CURLE_RECV_ERROR", 56);
    scope->createVariable("CURLE_SEND_ERROR", 55);
    scope->createVariable("CURLE_SHARE_IN_USE", 57);
    scope->createVariable("CURLE_SSL_CACERT", 60);
    scope->createVariable("CURLE_SSL_CERTPROBLEM", 58);
    scope->createVariable("CURLE_SSL_CIPHER", 59);
    scope->createVariable("CURLE_SSL_CONNECT_ERROR", 35);
    scope->createVariable("CURLE_SSL_ENGINE_NOTFOUND", 53);
    scope->createVariable("CURLE_SSL_ENGINE_SETFAILED", 54);
    scope->createVariable("CURLE_SSL_PEER_CERTIFICATE", 51);
    scope->createVariable("CURLE_SSL_PINNEDPUBKEYNOTMATCH", 90);
    scope->createVariable("CURLE_TELNET_OPTION_SYNTAX", 49);
    scope->createVariable("CURLE_TOO_MANY_REDIRECTS", 47);
    scope->createVariable("CURLE_UNKNOWN_TELNET_OPTION", 48);
    scope->createVariable("CURLE_UNSUPPORTED_PROTOCOL", 1);
    scope->createVariable("CURLE_URL_MALFORMAT", 3);
    scope->createVariable("CURLE_URL_MALFORMAT_USER", 4);
    scope->createVariable("CURLE_WRITE_ERROR", 23);
    scope->createVariable("CURLINFO_CONNECT_TIME", 3145733);
    scope->createVariable("CURLINFO_CONTENT_LENGTH_DOWNLOAD", 3145743);
    scope->createVariable("CURLINFO_CONTENT_LENGTH_UPLOAD", 3145744);
    scope->createVariable("CURLINFO_CONTENT_TYPE", 1048594);
    scope->createVariable("CURLINFO_EFFECTIVE_URL", 1048577);
    scope->createVariable("CURLINFO_FILETIME", 2097166);
    scope->createVariable("CURLINFO_HEADER_OUT", 2);
    scope->createVariable("CURLINFO_HEADER_SIZE", 2097163);
    scope->createVariable("CURLINFO_HTTP_CODE", 2097154);
    scope->createVariable("CURLINFO_LASTONE", 44);
    scope->createVariable("CURLINFO_NAMELOOKUP_TIME", 3145732);
    scope->createVariable("CURLINFO_PRETRANSFER_TIME", 3145734);
    scope->createVariable("CURLINFO_PRIVATE", 1048597);
    scope->createVariable("CURLINFO_REDIRECT_COUNT", 2097172);
    scope->createVariable("CURLINFO_REDIRECT_TIME", 3145747);
    scope->createVariable("CURLINFO_REQUEST_SIZE", 2097164);
    scope->createVariable("CURLINFO_SIZE_DOWNLOAD", 3145736);
    scope->createVariable("CURLINFO_SIZE_UPLOAD", 3145735);
    scope->createVariable("CURLINFO_SPEED_DOWNLOAD", 3145737);
    scope->createVariable("CURLINFO_SPEED_UPLOAD", 3145738);
    scope->createVariable("CURLINFO_SSL_VERIFYRESULT", 2097165);
    scope->createVariable("CURLINFO_STARTTRANSFER_TIME", 3145745);
    scope->createVariable("CURLINFO_TOTAL_TIME", 3145731);
    scope->createVariable("CURLMSG_DONE", 1);
    scope->createVariable("CURLVERSION_NOW", 3);
    scope->createVariable("CURLM_BAD_EASY_HANDLE", 2);
    scope->createVariable("CURLM_BAD_HANDLE", 1);
    scope->createVariable("CURLM_CALL_MULTI_PERFORM", -1);
    scope->createVariable("CURLM_INTERNAL_ERROR", 4);
    scope->createVariable("CURLM_OK", 0);
    scope->createVariable("CURLM_OUT_OF_MEMORY", 3);
    scope->createVariable("CURLM_ADDED_ALREADY", 7);
    scope->createVariable("CURLPROXY_HTTP", 0);
    scope->createVariable("CURLPROXY_SOCKS4", 4);
    scope->createVariable("CURLPROXY_SOCKS5", 5);
    scope->createVariable("CURLSHOPT_NONE", 0);
    scope->createVariable("CURLSHOPT_SHARE", 1);
    scope->createVariable("CURLSHOPT_UNSHARE", 2);
    scope->createVariable("CURL_HTTP_VERSION_1_0", 1);
    scope->createVariable("CURL_HTTP_VERSION_1_1", 2);
    scope->createVariable("CURL_HTTP_VERSION_NONE", 0);
    scope->createVariable("CURL_LOCK_DATA_COOKIE", 2);
    scope->createVariable("CURL_LOCK_DATA_DNS", 3);
    scope->createVariable("CURL_LOCK_DATA_SSL_SESSION", 4);
    scope->createVariable("CURL_NETRC_IGNORED", 0);
    scope->createVariable("CURL_NETRC_OPTIONAL", 1);
    scope->createVariable("CURL_NETRC_REQUIRED", 2);
    scope->createVariable("CURL_SSLVERSION_DEFAULT", 0);
    scope->createVariable("CURL_SSLVERSION_SSLv2", 2);
    scope->createVariable("CURL_SSLVERSION_SSLv3", 3);
    scope->createVariable("CURL_SSLVERSION_TLSv1", 1);
    scope->createVariable("CURL_TIMECOND_IFMODSINCE", 1);
    scope->createVariable("CURL_TIMECOND_IFUNMODSINCE", 2);
    scope->createVariable("CURL_TIMECOND_LASTMOD", 3);
    scope->createVariable("CURL_TIMECOND_NONE", 0);
    scope->createVariable("CURL_VERSION_IPV6", 1);
    scope->createVariable("CURL_VERSION_KERBEROS4", 2);
    scope->createVariable("CURL_VERSION_LIBZ", 8);
    scope->createVariable("CURL_VERSION_SSL", 4);
    scope->createVariable("CURLOPT_HTTPAUTH", 107);
    scope->createVariable("CURLAUTH_ANY", -17);
    scope->createVariable("CURLAUTH_ANYSAFE", -18);
    scope->createVariable("CURLAUTH_BASIC", 1);
    scope->createVariable("CURLAUTH_DIGEST", 2);
    scope->createVariable("CURLAUTH_GSSNEGOTIATE", 4);
    scope->createVariable("CURLAUTH_NONE", 0);
    scope->createVariable("CURLAUTH_NTLM", 8);
    scope->createVariable("CURLINFO_HTTP_CONNECTCODE", 2097174);
    scope->createVariable("CURLOPT_FTP_CREATE_MISSING_DIRS", 110);
    scope->createVariable("CURLOPT_PROXYAUTH", 111);
    scope->createVariable("CURLE_FILESIZE_EXCEEDED", 63);
    scope->createVariable("CURLE_LDAP_INVALID_URL", 62);
    scope->createVariable("CURLINFO_HTTPAUTH_AVAIL", 2097175);
    scope->createVariable("CURLINFO_RESPONSE_CODE", 2097154);
    scope->createVariable("CURLINFO_PROXYAUTH_AVAIL", 2097176);
    scope->createVariable("CURLOPT_FTP_RESPONSE_TIMEOUT", 112);
    scope->createVariable("CURLOPT_IPRESOLVE", 113);
    scope->createVariable("CURLOPT_MAXFILESIZE", 114);
    scope->createVariable("CURL_IPRESOLVE_V4", 1);
    scope->createVariable("CURL_IPRESOLVE_V6", 2);
    scope->createVariable("CURL_IPRESOLVE_WHATEVER", 0);
    scope->createVariable("CURLE_FTP_SSL_FAILED", 64);
    scope->createVariable("CURLFTPSSL_ALL", 3);
    scope->createVariable("CURLFTPSSL_CONTROL", 2);
    scope->createVariable("CURLFTPSSL_NONE", 0);
    scope->createVariable("CURLFTPSSL_TRY", 1);
    scope->createVariable("CURLOPT_FTP_SSL", 119);
    scope->createVariable("CURLOPT_NETRC_FILE", 10118);
    scope->createVariable("CURLFTPAUTH_DEFAULT", 0);
    scope->createVariable("CURLFTPAUTH_SSL", 1);
    scope->createVariable("CURLFTPAUTH_TLS", 2);
    scope->createVariable("CURLOPT_FTPSSLAUTH", 129);
    scope->createVariable("CURLOPT_FTP_ACCOUNT", 10134);
    scope->createVariable("CURLOPT_TCP_NODELAY", 121);
    scope->createVariable("CURLINFO_OS_ERRNO", 2097177);
    scope->createVariable("CURLINFO_NUM_CONNECTS", 2097178);
    scope->createVariable("CURLINFO_SSL_ENGINES", 4194331);
    scope->createVariable("CURLINFO_COOKIELIST", 4194332);
    scope->createVariable("CURLOPT_COOKIELIST", 10135);
    scope->createVariable("CURLOPT_IGNORE_CONTENT_LENGTH", 136);
    scope->createVariable("CURLOPT_FTP_SKIP_PASV_IP", 137);
    scope->createVariable("CURLOPT_FTP_FILEMETHOD", 138);
    scope->createVariable("CURLOPT_CONNECT_ONLY", 141);
    scope->createVariable("CURLOPT_LOCALPORT", 139);
    scope->createVariable("CURLOPT_LOCALPORTRANGE", 140);
    scope->createVariable("CURLFTPMETHOD_MULTICWD", 1);
    scope->createVariable("CURLFTPMETHOD_NOCWD", 2);
    scope->createVariable("CURLFTPMETHOD_SINGLECWD", 3);
    scope->createVariable("CURLINFO_FTP_ENTRY_PATH", 1048606);
    scope->createVariable("CURLOPT_FTP_ALTERNATIVE_TO_USER", 10147);
    scope->createVariable("CURLOPT_MAX_RECV_SPEED_LARGE", 30146);
    scope->createVariable("CURLOPT_MAX_SEND_SPEED_LARGE", 30145);
    scope->createVariable("CURLE_SSL_CACERT_BADFILE", 77);
    scope->createVariable("CURLOPT_SSL_SESSIONID_CACHE", 150);
    scope->createVariable("CURLMOPT_PIPELINING", 3);
    scope->createVariable("CURLE_SSH", 79);
    scope->createVariable("CURLOPT_FTP_SSL_CCC", 154);
    scope->createVariable("CURLOPT_SSH_AUTH_TYPES", 151);
    scope->createVariable("CURLOPT_SSH_PRIVATE_KEYFILE", 10153);
    scope->createVariable("CURLOPT_SSH_PUBLIC_KEYFILE", 10152);
    scope->createVariable("CURLFTPSSL_CCC_ACTIVE", 2);
    scope->createVariable("CURLFTPSSL_CCC_NONE", 0);
    scope->createVariable("CURLFTPSSL_CCC_PASSIVE", 1);
    scope->createVariable("CURLOPT_CONNECTTIMEOUT_MS", 156);
    scope->createVariable("CURLOPT_HTTP_CONTENT_DECODING", 158);
    scope->createVariable("CURLOPT_HTTP_TRANSFER_DECODING", 157);
    scope->createVariable("CURLOPT_TIMEOUT_MS", 155);
    scope->createVariable("CURLMOPT_MAXCONNECTS", 6);
    scope->createVariable("CURLOPT_KRBLEVEL", 10063);
    scope->createVariable("CURLOPT_NEW_DIRECTORY_PERMS", 160);
    scope->createVariable("CURLOPT_NEW_FILE_PERMS", 159);
    scope->createVariable("CURLOPT_APPEND", 50);
    scope->createVariable("CURLOPT_DIRLISTONLY", 48);
    scope->createVariable("CURLOPT_USE_SSL", 119);
    scope->createVariable("CURLUSESSL_ALL", 3);
    scope->createVariable("CURLUSESSL_CONTROL", 2);
    scope->createVariable("CURLUSESSL_NONE", 0);
    scope->createVariable("CURLUSESSL_TRY", 1);
    scope->createVariable("CURLOPT_SSH_HOST_PUBLIC_KEY_MD5", 10162);
    scope->createVariable("CURLOPT_PROXY_TRANSFER_MODE", 166);
    scope->createVariable("CURLPAUSE_ALL", 5);
    scope->createVariable("CURLPAUSE_CONT", 0);
    scope->createVariable("CURLPAUSE_RECV", 1);
    scope->createVariable("CURLPAUSE_RECV_CONT", 0);
    scope->createVariable("CURLPAUSE_SEND", 4);
    scope->createVariable("CURLPAUSE_SEND_CONT", 0);
    scope->createVariable("CURL_READFUNC_PAUSE", 268435457);
    scope->createVariable("CURL_WRITEFUNC_PAUSE", 268435457);
    scope->createVariable("CURLPROXY_SOCKS4A", 6);
    scope->createVariable("CURLPROXY_SOCKS5_HOSTNAME", 7);
    scope->createVariable("CURLINFO_REDIRECT_URL", 1048607);
    scope->createVariable("CURLINFO_APPCONNECT_TIME", 3145761);
    scope->createVariable("CURLINFO_PRIMARY_IP", 1048608);
    scope->createVariable("CURLOPT_ADDRESS_SCOPE", 171);
    scope->createVariable("CURLOPT_CRLFILE", 10169);
    scope->createVariable("CURLOPT_ISSUERCERT", 10170);
    scope->createVariable("CURLOPT_KEYPASSWD", 10026);
    scope->createVariable("CURLSSH_AUTH_ANY", -1);
    scope->createVariable("CURLSSH_AUTH_DEFAULT", -1);
    scope->createVariable("CURLSSH_AUTH_HOST", 4);
    scope->createVariable("CURLSSH_AUTH_KEYBOARD", 8);
    scope->createVariable("CURLSSH_AUTH_NONE", 0);
    scope->createVariable("CURLSSH_AUTH_PASSWORD", 2);
    scope->createVariable("CURLSSH_AUTH_PUBLICKEY", 1);
    scope->createVariable("CURLINFO_CERTINFO", 4194338);
    scope->createVariable("CURLOPT_CERTINFO", 172);
    scope->createVariable("CURLOPT_PASSWORD", 10174);
    scope->createVariable("CURLOPT_POSTREDIR", 161);
    scope->createVariable("CURLOPT_PROXYPASSWORD", 10176);
    scope->createVariable("CURLOPT_PROXYUSERNAME", 10175);
    scope->createVariable("CURLOPT_USERNAME", 10173);
    scope->createVariable("CURL_REDIR_POST_301", 1);
    scope->createVariable("CURL_REDIR_POST_302", 2);
    scope->createVariable("CURL_REDIR_POST_ALL", 7);
    scope->createVariable("CURLAUTH_DIGEST_IE", 16);
    scope->createVariable("CURLINFO_CONDITION_UNMET", 2097187);
    scope->createVariable("CURLOPT_NOPROXY", 10177);
    scope->createVariable("CURLOPT_PROTOCOLS", 181);
    scope->createVariable("CURLOPT_REDIR_PROTOCOLS", 182);
    scope->createVariable("CURLOPT_SOCKS5_GSSAPI_NEC", 180);
    scope->createVariable("CURLOPT_SOCKS5_GSSAPI_SERVICE", 10179);
    scope->createVariable("CURLOPT_TFTP_BLKSIZE", 178);
    scope->createVariable("CURLPROTO_ALL", -1);
    scope->createVariable("CURLPROTO_DICT", 512);
    scope->createVariable("CURLPROTO_FILE", 1024);
    scope->createVariable("CURLPROTO_FTP", 4);
    scope->createVariable("CURLPROTO_FTPS", 8);
    scope->createVariable("CURLPROTO_HTTP", 1);
    scope->createVariable("CURLPROTO_HTTPS", 2);
    scope->createVariable("CURLPROTO_LDAP", 128);
    scope->createVariable("CURLPROTO_LDAPS", 256);
    scope->createVariable("CURLPROTO_SCP", 16);
    scope->createVariable("CURLPROTO_SFTP", 32);
    scope->createVariable("CURLPROTO_TELNET", 64);
    scope->createVariable("CURLPROTO_TFTP", 2048);
    scope->createVariable("CURLPROXY_HTTP_1_0", 1);
    scope->createVariable("CURLFTP_CREATE_DIR", 1);
    scope->createVariable("CURLFTP_CREATE_DIR_NONE", 0);
    scope->createVariable("CURLFTP_CREATE_DIR_RETRY", 2);
    scope->createVariable("CURLOPT_SSH_KNOWNHOSTS", 10183);
    scope->createVariable("CURLINFO_RTSP_CLIENT_CSEQ", 2097189);
    scope->createVariable("CURLINFO_RTSP_CSEQ_RECV", 2097191);
    scope->createVariable("CURLINFO_RTSP_SERVER_CSEQ", 2097190);
    scope->createVariable("CURLINFO_RTSP_SESSION_ID", 1048612);
    scope->createVariable("CURLOPT_FTP_USE_PRET", 188);
    scope->createVariable("CURLOPT_MAIL_FROM", 10186);
    scope->createVariable("CURLOPT_MAIL_RCPT", 10187);
    scope->createVariable("CURLOPT_RTSP_CLIENT_CSEQ", 193);
    scope->createVariable("CURLOPT_RTSP_REQUEST", 189);
    scope->createVariable("CURLOPT_RTSP_SERVER_CSEQ", 194);
    scope->createVariable("CURLOPT_RTSP_SESSION_ID", 10190);
    scope->createVariable("CURLOPT_RTSP_STREAM_URI", 10191);
    scope->createVariable("CURLOPT_RTSP_TRANSPORT", 10192);
    scope->createVariable("CURLPROTO_IMAP", 4096);
    scope->createVariable("CURLPROTO_IMAPS", 8192);
    scope->createVariable("CURLPROTO_POP3", 16384);
    scope->createVariable("CURLPROTO_POP3S", 32768);
    scope->createVariable("CURLPROTO_RTSP", 262144);
    scope->createVariable("CURLPROTO_SMTP", 65536);
    scope->createVariable("CURLPROTO_SMTPS", 131072);
    scope->createVariable("CURL_RTSPREQ_ANNOUNCE", 3);
    scope->createVariable("CURL_RTSPREQ_DESCRIBE", 2);
    scope->createVariable("CURL_RTSPREQ_GET_PARAMETER", 8);
    scope->createVariable("CURL_RTSPREQ_OPTIONS", 1);
    scope->createVariable("CURL_RTSPREQ_PAUSE", 6);
    scope->createVariable("CURL_RTSPREQ_PLAY", 5);
    scope->createVariable("CURL_RTSPREQ_RECEIVE", 11);
    scope->createVariable("CURL_RTSPREQ_RECORD", 10);
    scope->createVariable("CURL_RTSPREQ_SET_PARAMETER", 9);
    scope->createVariable("CURL_RTSPREQ_SETUP", 4);
    scope->createVariable("CURL_RTSPREQ_TEARDOWN", 7);
    scope->createVariable("CURLINFO_LOCAL_IP", 1048617);
    scope->createVariable("CURLINFO_LOCAL_PORT", 2097194);
    scope->createVariable("CURLINFO_PRIMARY_PORT", 2097192);
    scope->createVariable("CURLOPT_FNMATCH_FUNCTION", 20200);
    scope->createVariable("CURLOPT_WILDCARDMATCH", 197);
    scope->createVariable("CURLPROTO_RTMP", 524288);
    scope->createVariable("CURLPROTO_RTMPE", 2097152);
    scope->createVariable("CURLPROTO_RTMPS", 8388608);
    scope->createVariable("CURLPROTO_RTMPT", 1048576);
    scope->createVariable("CURLPROTO_RTMPTE", 4194304);
    scope->createVariable("CURLPROTO_RTMPTS", 16777216);
    scope->createVariable("CURL_FNMATCHFUNC_FAIL", 2);
    scope->createVariable("CURL_FNMATCHFUNC_MATCH", 0);
    scope->createVariable("CURL_FNMATCHFUNC_NOMATCH", 1);
    scope->createVariable("CURLPROTO_GOPHER", 33554432);
    scope->createVariable("CURLAUTH_ONLY", 2147483648);
    scope->createVariable("CURLOPT_RESOLVE", 10203);
    scope->createVariable("CURLOPT_TLSAUTH_PASSWORD", 10205);
    scope->createVariable("CURLOPT_TLSAUTH_TYPE", 10206);
    scope->createVariable("CURLOPT_TLSAUTH_USERNAME", 10204);
    scope->createVariable("CURL_TLSAUTH_SRP", 1);
    scope->createVariable("CURLOPT_ACCEPT_ENCODING", 10102);
    scope->createVariable("CURLOPT_TRANSFER_ENCODING", 207);
    scope->createVariable("CURLAUTH_NTLM_WB", 32);
    scope->createVariable("CURLGSSAPI_DELEGATION_FLAG", 2);
    scope->createVariable("CURLGSSAPI_DELEGATION_POLICY_FLAG", 1);
    scope->createVariable("CURLOPT_GSSAPI_DELEGATION", 210);
    scope->createVariable("CURLOPT_ACCEPTTIMEOUT_MS", 212);
    scope->createVariable("CURLOPT_DNS_SERVERS", 10211);
    scope->createVariable("CURLOPT_MAIL_AUTH", 10217);
    scope->createVariable("CURLOPT_SSL_OPTIONS", 216);
    scope->createVariable("CURLOPT_TCP_KEEPALIVE", 213);
    scope->createVariable("CURLOPT_TCP_KEEPIDLE", 214);
    scope->createVariable("CURLOPT_TCP_KEEPINTVL", 215);
    scope->createVariable("CURLSSLOPT_ALLOW_BEAST", 1);
    scope->createVariable("CURL_REDIR_POST_303", 4);
    scope->createVariable("CURLSSH_AUTH_AGENT", 16);
    scope->createVariable("CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE", 30010);
    scope->createVariable("CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE", 30009);
    scope->createVariable("CURLMOPT_MAX_HOST_CONNECTIONS", 7);
    scope->createVariable("CURLMOPT_MAX_PIPELINE_LENGTH", 8);
    scope->createVariable("CURLMOPT_MAX_TOTAL_CONNECTIONS", 13);
    scope->createVariable("CURLOPT_SASL_IR", 218);
    scope->createVariable("CURLOPT_DNS_INTERFACE", 10221);
    scope->createVariable("CURLOPT_DNS_LOCAL_IP4", 10222);
    scope->createVariable("CURLOPT_DNS_LOCAL_IP6", 10223);
    scope->createVariable("CURLOPT_XOAUTH2_BEARER", 10220);
    scope->createVariable("CURL_HTTP_VERSION_2_0", 3);
    scope->createVariable("CURL_VERSION_HTTP2", 65536);
    scope->createVariable("CURLOPT_LOGIN_OPTIONS", 10224);
    scope->createVariable("CURL_SSLVERSION_TLSv1_0", 4);
    scope->createVariable("CURL_SSLVERSION_TLSv1_1", 5);
    scope->createVariable("CURL_SSLVERSION_TLSv1_2", 6);
    scope->createVariable("CURLOPT_EXPECT_100_TIMEOUT_MS", 227);
    scope->createVariable("CURLOPT_SSL_ENABLE_ALPN", 226);
    scope->createVariable("CURLOPT_SSL_ENABLE_NPN", 225);
    scope->createVariable("CURLHEADER_SEPARATE", 1);
    scope->createVariable("CURLHEADER_UNIFIED", 0);
    scope->createVariable("CURLOPT_HEADEROPT", 229);
    scope->createVariable("CURLOPT_PROXYHEADER", 10228);
    scope->createVariable("CURLAUTH_NEGOTIATE", 4);
    scope->createVariable("CURLOPT_PINNEDPUBLICKEY", 10230);
    scope->createVariable("CURLOPT_UNIX_SOCKET_PATH", 10231);
    scope->createVariable("CURLPROTO_SMB", 67108864);
    scope->createVariable("CURLPROTO_SMBS", 134217728);
    scope->createVariable("CURLOPT_SSL_VERIFYSTATUS", 232);
    scope->createVariable("CURLOPT_PATH_AS_IS", 234);
    scope->createVariable("CURLOPT_SSL_FALSESTART", 233);
    scope->createVariable("CURL_HTTP_VERSION_2", 3);
    scope->createVariable("CURLOPT_PIPEWAIT", 237);
    scope->createVariable("CURLOPT_PROXY_SERVICE_NAME", 10235);
    scope->createVariable("CURLOPT_SERVICE_NAME", 10236);
    scope->createVariable("CURLPIPE_NOTHING", 0);
    scope->createVariable("CURLPIPE_HTTP1", 1);
    scope->createVariable("CURLPIPE_MULTIPLEX", 2);
    scope->createVariable("CURLSSLOPT_NO_REVOKE", 2);
    scope->createVariable("CURLOPT_DEFAULT_PROTOCOL", 10238);
    scope->createVariable("CURLOPT_STREAM_WEIGHT", 239);
    scope->createVariable("CURL_HTTP_VERSION_2TLS", 4);
    scope->createVariable("CURLOPT_SAFE_UPLOAD", -1);
}

void CurlObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Curl
     * 
     * Responsible for handling all curl operations. Creating a new instance of this class will create a new Curl object allowing you to talk with a http server or use other services related to Curl
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Curl");
    c->setDescriptorObject(std::make_shared<CurlObject>(c));

    /**
     * @class Curl
     * 
     * Constructs this Curl object and initialises curl internally.
     * Throws an IOException on failure to setup curl
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        std::shared_ptr<CurlObject> curl_obj = std::dynamic_pointer_cast<CurlObject>(object);
        curl_obj->curl = curl_easy_init();
        if (!curl_obj->curl)
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Issue initialising curl internally", interpreter->getStackTraceLog());

        curl_easy_setopt(curl_obj->curl, CURLOPT_WRITEFUNCTION, NetworkModule::CurlWriteCallback);
        curl_easy_setopt(curl_obj->curl, CURLOPT_WRITEDATA, &curl_obj->write_data);
    });

    /**
     * @class Curl
     * 
     * Sets an option for this curl. The _option must be a valid curl option. The value provided must also be valid for the given option
     * 
     * Seek here: https://curl.haxx.se/libcurl/c/easy_setopt_options.html for a list of valid options for Curl
     * Note: not all options are implemented within marble you will receive an UnimplementedException if this is the case
     * 
     * function setopt(number _option, string value) : void
     */
    c->registerFunction("setopt", {VarType::fromString("number"), VarType::fromString("string")}, VarType::fromString("void"), CurlObject::Curl_setopt);

    /**
     * @class Curl
     * 
     * Sets an option for this curl. The _option must be a valid curl option. The value provided must also be valid for the given option
     * 
     * Seek here: https://curl.haxx.se/libcurl/c/easy_setopt_options.html for a list of valid options for Curl
     * Note: not all options are implemented within marble you will receive an UnimplementedException if this is the case
     * 
     * function setopt(number _option, number value) : void
     */
    c->registerFunction("setopt", {VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("void"), CurlObject::Curl_setopt);

    /**
     * @class Curl
     * 
     * Sets an option for this curl. The _option must be a valid curl option. The value provided must also be valid for the given option
     * 
     * Seek here: https://curl.haxx.se/libcurl/c/easy_setopt_options.html for a list of valid options for Curl
     * Note: not all options are implemented within marble you will receive an UnimplementedException if this is the case
     * 
     * function setoptArray(number _option, string[] value) : void
     */
    c->registerFunction("setoptArray", {VarType::fromString("number"), VarType::fromString("string[]")}, VarType::fromString("void"), CurlObject::Curl_setopt);

    /**
     * @class Curl
     * Executes this Curl request and returns the response body
     * function execute() : string
     */
    c->registerFunction("execute", {}, VarType::fromString("string"), CurlObject::Curl_execute);

    /**
     * @class Curl
     * Request internal information from the curl session with this function.
     * The info passed is based on the curl doc. Seek available information: https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html
     * function getinfo(number info) : number
     */
    c->registerFunction("getinfo", {VarType::fromString("number")}, VarType::fromString("number"), CurlObject::Curl_getinfo);

    /**
     * @class Curl
     * 
     * Closes this curl connection
     * function close() : void
     */
    c->registerFunction("close", {}, VarType::fromString("void"), CurlObject::Curl_close);
}

void CurlObject::Curl_setopt(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<CurlObject> curl_obj = std::dynamic_pointer_cast<CurlObject>(object);
    if (values[0].dvalue == CURLOPT_UPLOAD)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Uploading files with CURL has been disabled for now while we come up with a way for it to work well with the permission system", interpreter->getStackTraceLog());

    if (startsWith(values[1].svalue, "file://"))
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "File access is not allowed with the CURL module for now as we need to come up with a way for it to work well with the permission system", interpreter->getStackTraceLog());

    if (values[1].type == VALUE_TYPE_ARRAY)
    {
        struct curl_slist *slist = NULL;
        if (values[1].avalue == NULL)
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("NullPointerException"))), "The provided array is null for use in curl", interpreter->getStackTraceLog());

        Variable *avars = values[1].avalue->variables;
        for (int i = 0; i < values[1].avalue->count; i++)
        {
            Value aval = avars[i].value;
            slist = curl_slist_append(slist, aval.svalue.c_str());
        }

        curl_obj->lists_to_free.push_back(slist);

        curl_easy_setopt(curl_obj->curl, (CURLoption)values[0].dvalue, slist);
    }
    else
    {
        std::string opt_value;
        if (values[1].type == VALUE_TYPE_NUMBER)
            opt_value = std::to_string(values[1].dvalue);
        else
            opt_value = values[1].svalue;
        
        curl_easy_setopt(curl_obj->curl, (CURLoption)values[0].dvalue, opt_value.c_str());
    }
}

void CurlObject::Curl_execute(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Ensure we have permission
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_CURL_PERMISSION_REQUIRED);

    std::shared_ptr<CurlObject> curl_obj = std::dynamic_pointer_cast<CurlObject>(object);
    // Reset write_data
    curl_obj->write_data = "";

    CURLcode res = curl_easy_perform(curl_obj->curl);

    // Free the lists created for this execution
    if (!curl_obj->lists_to_free.empty())
    {
        for (struct curl_slist *slist : curl_obj->lists_to_free)
            curl_slist_free_all(slist);
    }
    curl_obj->lists_to_free.erase(curl_obj->lists_to_free.begin(), curl_obj->lists_to_free.end());

    if (res != CURLE_OK)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Issue executing your CURL request. Curl error code: " + std::to_string(res), interpreter->getStackTraceLog());

    // Return the data that was written
    return_value->set(curl_obj->write_data);
}

void CurlObject::Curl_close(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<CurlObject> curl_obj = std::dynamic_pointer_cast<CurlObject>(object);
    if (curl_obj->curl != NULL)
    {
        curl_easy_cleanup(curl_obj->curl);
        curl_obj->curl = NULL;
    }
}

void CurlObject::Curl_getinfo(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CurlObject> curl_obj = std::dynamic_pointer_cast<CurlObject>(object);
    long val;
    curl_easy_getinfo(curl_obj->curl, (CURLINFO)values[0].dvalue, &val);
    return_value->set(val);
}