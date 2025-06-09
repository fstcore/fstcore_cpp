#include "stdafx.h"

using namespace std;

class Encoder {

public:
	string utf8_encode(const std::wstring& wstr);
	wstring utf8_decode(const std::string& str);
	string unicode_to_ansi(const std::wstring& wstr);
	wstring ansi_to_unicode(const std::string& str);
	string base64_encode(string value);
	string base64_decode(string value);
	string url_encode(const string& value);
	string url_decode(const string& value);
	string md5hash(string e);
	string randomstring();

};

