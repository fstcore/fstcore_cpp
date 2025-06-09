#include <vcclr.h>
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cstring>  
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <regex>
#include <curl/curl.h>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/random.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>
#include "ConsoleLog.h"

using namespace std;
using namespace boost;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace System;

class HttpRequest {

private:
	typedef enum proxy_type { http = 0, https = 1, socks5 = 2, socks4 = 3, socks4a = 4 };
	typedef enum post_type { raw = 0, data = 1, form = 2 };
	typedef enum request_type { GET = 0, POST = 1, PUT = 2, PROFIND = 3, HEAD = 4, DEL = 5 };
	string url;
	string header;
	string cookie;
	int timeout = 25000;
	string postdata;
	string cookiefile;
	char response_uri = NULL;
	long response_code;
	string response_header;
	string response_body;
	map<string, string> response;
	gcroot<ConsoleLog^> log = gcnew ConsoleLog();
	CURL *curl;
	CURLcode res;
	struct curl_slist *slist = NULL;
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	CURL *curl_handle = curl_easy_init();
	map<string, proxy_type> enum_proxy;
	map<string, post_type> enum_post;
	map<string, request_type> enum_request;

private:
	static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata);
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	
public:
	HttpRequest();
	string md5hash(string e);
	string randomstring();
	void set_url(string url);
	void set_timeout(int t);
	void add_header(string h);
	void add_cookie(string c);
	string get_cookies();
	void set_proxy(string type, string ip, int port, char auth);
	void set_postdata(string type, string d);
	map<string, string> request(string type);
};
