#pragma once
#include "pch.h"

using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;

class crawl {

public:
	static string proxy_type;
	static string proxy_host;
	static int proxy_port;
	static string proxy_username;
	static string proxy_password;
	static int max_deep;
	static bool with_form;
	static bool with_href;

private:
	map<string, string> http_get(string url, string header);
	map<string, string> http_post(string url, string header, string data, string d_type);
	vector<string> manage_array(map<string, string> r, vector<string> returned);

public:
	crawl();
	void set_max(int md);
	map<string, string> parse_links(string url);
	map<string, string> parse_form(string url);
	vector<string> run(string url);
};
