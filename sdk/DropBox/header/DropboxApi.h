#pragma once
#include "pch.h"

using namespace std;

class DropboxApi {

public:
	static string proxy_type;
	static string proxy_host;
	static int proxy_port;
	static string proxy_username;
	static string proxy_password;
	static string key;

private:
	map<string, string> http_get(string url, string header);
	map<string, string> http_post(string url, string header, string data, string d_type);

public:
	DropboxApi();
	void set_key(string key);
	void create_folder(string target);
	void delete_bulk(string target);
	void delete_file(string filepath);
	void upload(string filepath, string target);
	void download_bulk(string target, string destination);
	string download(string filepath);
	vector<string> list(string target);
	vector<string> cursor_continue(string cursor);
};