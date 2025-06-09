#pragma once
#include "pch.h"

using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;

class HtmlUtility {

public:
	HtmlUtility();
	string url_filter_extend(string url, string link);
	string url_filter(string url, string link);
	bool filter_extension(string e);
	string clear_domain(string host);
	string arrange_slash(string url);
	vector<string> html_array(string html);
};
