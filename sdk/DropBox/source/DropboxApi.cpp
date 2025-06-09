#include "pch.h"
#include "DropboxApi.h"

string DropboxApi::proxy_type;
string DropboxApi::proxy_host;
int DropboxApi::proxy_port;
string DropboxApi::proxy_username;
string DropboxApi::proxy_password;
string DropboxApi::key;

DropboxApi::DropboxApi() {
}

map<string, string> DropboxApi::http_get(string url, string header) {
	HttpRequestCurl http;
	http.set_url(url);
	http.set_timeout(30000);
	http.add_header(header);
	if (proxy_type.length() > 0 && proxy_username.length() > 0) {
		string proxyauth = to_string(proxy_username + ":" + proxy_password);
		http.set_proxy(proxy_type, proxy_host, proxy_port, (char)proxyauth.c_str());
	}
	else if (proxy_type.length() > 0) {
		http.set_proxy(proxy_type, proxy_host, proxy_port, NULL);
	}
	return http.request("GET");
}

map<string, string> DropboxApi::http_post(string url, string header, string data, string d_type) {
	HttpRequestCurl http;
	http.set_url(url);
	http.set_timeout(30000);
	http.add_header(header);
	if (proxy_type.length() > 0 && proxy_username.length() > 0) {
		string proxyauth = to_string(proxy_username + ":" + proxy_password);
		http.set_proxy(proxy_type, proxy_host, proxy_port, (char)proxyauth.c_str());
	}
	else if (proxy_type.length() > 0) {
		http.set_proxy(proxy_type, proxy_host, proxy_port, NULL);
	}
	http.set_postdata(d_type, data);
	return http.request("POST");
}

void DropboxApi::create_folder(string target) {
	json jsonobj;
	jsonobj.emplace("path", target);
	jsonobj.emplace("autorename", false);
	string jsonstr = jsonobj.dump();
	string header = "Authorization->" + this->key + "&Content-Type->application/json";
	map<string, string> res = DropboxApi::http_post("https://api.dropboxapi.com/2/files/create_folder_v2", header, jsonstr.c_str(), "raw");
}

void DropboxApi::set_key(string key) {
	this->key = key;
}

void DropboxApi::delete_bulk(string target) {
	vector<string> files = DropboxApi::list(target);
	for (auto f : files) {
		DropboxApi::delete_file(target + "/" + f);
	}
}

void DropboxApi::delete_file(string filepath) {
	json jsonobj;
	jsonobj.emplace("path", filepath);
	string jsonstr = jsonobj.dump();
	string header = "Authorization->" + this->key + "&Content-Type->application/json";
	map<string, string> res = DropboxApi::http_post("https://api.dropboxapi.com/2/files/delete_v2", header, jsonstr.c_str(), "raw");
}

void DropboxApi::upload(string filepath, string target) {
	json jsonobj;
	jsonobj.emplace("path", filepath);
	jsonobj.emplace("mode", "add");
	jsonobj.emplace("autorename", true);
	jsonobj.emplace("mute", false);
	jsonobj.emplace("strict_conflict", false);
	string jsonstr = jsonobj.dump();
	string header = "Authorization->" + this->key + "&Dropbox-API-Arg->" + jsonstr + "&Content-Type->application/octet-stream";
	fstream fopen;
	fopen.open(filepath, ios::in);
	char x;
	while (1) {
		fopen >> x;
		if (fopen.eof())
			break;
	}
	fopen.close();
	map<string, string> res = DropboxApi::http_post("https://content.dropboxapi.com/2/files/upload", header, to_string(x), "raw");
}

void DropboxApi::download_bulk(string target, string destination) {
	vector <string> files = DropboxApi::list(target);
	for (auto f : files) {
		string data = DropboxApi::download(target + "/" + f);
		fstream fopen;
		fopen.open(destination + "/" + f, ios::out);
		fopen << data;
		fopen.close();
	}
}

string DropboxApi::download(string filepath) {
	json jsonobj;
	jsonobj.emplace("path", filepath);
	string jsonstr = jsonobj.dump();
	string header = "Authorization->" + this->key + "&Dropbox-API-Arg->" + jsonstr + "&Content-Type->application/octet-stream";
	map<string, string> res = DropboxApi::http_get("https://content.dropboxapi.com/2/files/download", header);
	return res["body"];
}

vector<string> DropboxApi::list(string target) {
	vector<string> returned;
	json jsonobj;
	jsonobj.emplace("path", target);
	jsonobj.emplace("recursive", true);
	jsonobj.emplace("include_media_info", true);
	jsonobj.emplace("include_deleted", false);
	jsonobj.emplace("include_has_explicit_shared_members", false);
	jsonobj.emplace("include_mounted_folders", true);
	jsonobj.emplace("include_non_downloadable_files", true);
	jsonobj.emplace("limit", 100);
	string jsonstr = jsonobj.dump();
	string header = "Authorization->" + this->key + "&Content-Type->application/json";
	map<string, string> res = DropboxApi::http_post("https://api.dropboxapi.com/2/files/list_folder", header, jsonstr.c_str(), "raw");
	if (icontains(res["body"].c_str(), "{")) {
		json jsond = json::parse(res["body"].c_str());
		for (auto j : jsond["entries"]) {
			json jsontmp;
			jsontmp.emplace("name", j["name"]);
			jsontmp.emplace("file_path", j["path_lower"]);
			jsontmp.emplace("size", j["size"]);
			jsontmp.emplace("date", j["server_modified"]);
			returned.push_back(jsontmp.dump());
		}
		if (jsond["has_more"]) {
			vector<string> files = DropboxApi::cursor_continue(jsond["cursor"]);
			returned.insert(returned.end(), files.begin(), files.end());
		}
	}
	else {
	}
	return returned;
}

vector<string> DropboxApi::cursor_continue(string cursor) {
	vector<string> returned;
	while (true) {
		json jsonobj;
		jsonobj.emplace("cursor", cursor);
		string jsonstr = jsonobj.dump();
		string header = "Authorization->" + this->key + "&Content-Type->application/json";
		map<string, string> res = DropboxApi::http_post("https://api.dropboxapi.com/2/files/list_folder/continue", header, jsonstr.c_str(), "raw");
		if (icontains(res["body"].c_str(), "{")) {
			json jsond = json::parse(res["body"].c_str());
			for (json j : jsond["entries"]) {
				if (j[".tag"] == "file" && j["name"] == false) {
					json jsontmp;
					jsontmp.emplace("name", j["name"]);
					jsontmp.emplace("file_path", j["path_lower"]);
					jsontmp.emplace("size", j["size"]);
					jsontmp.emplace("date", j["server_modified"]);
					returned.push_back(jsontmp.dump());
				}
			}
			if (jsond["has_more"]) {
				vector<string> files = cursor_continue(jsond["cursor"]);
				returned.insert(returned.end(), files.begin(), files.end());
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
	return returned;
}
