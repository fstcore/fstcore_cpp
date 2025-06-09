#include "pch.h"
#include "crawl.h"

string crawl::proxy_type;
string crawl::proxy_host;
int crawl::proxy_port;
string crawl::proxy_username;
string crawl::proxy_password;
int crawl::max_deep = 1;
bool crawl::with_form = false;
bool crawl::with_href = true;

crawl::crawl() {
}

map<string, string> crawl::http_get(string url, string header) {
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

map<string, string> crawl::http_post(string url, string header, string data, string d_type) {
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

void crawl::set_max(int max) {
	crawl::max_deep = max;
}

map<string, string> crawl::parse_links(string url) {
	map<string, string> returned;
	json links = json::array();
	HostUtility hostutility;
	EncoderUtility encoder;
	map<string, string> domaininfo = hostutility.domain_info(url);
	map<string, string> res = http_get(url, DEFAULT_HTTP_HEADER);
	if (res["code"] == "200") {
		HtmlUtility html_;
		vector<string> contents = html_.html_array(res["body"]);
		for (string c : contents) {
			cout << c << endl;
			boost::smatch sm;
			boost::regex p{"href\\=\"(.*?)\"|href\\=\'(.*?)\'"};
			boost::regex_search(c, sm, p);
			for (size_t i = 1; i < sm.size(); i++) {
				if ((sm[int(i)] == "") == false && (sm[int(i)] == "#") == false && (icontains(sm[int(i)], "javascript:") == false)) {
					if (html_.filter_extension(sm[int(i)]) == false) {
						string link_dec_ = encoder.url_decode(html_.url_filter(res["uri"], sm[int(i)])).c_str();
						//string link = encoder.unicode_to_ansi(ctx.marshal_as<wstring>(link_dec_));
						json jsonobj;
						jsonobj.emplace("url", link_dec_);
						if (links.contains(to_string(jsonobj)) == false) {
							links.push_back(jsonobj);
						}						
					}
				}
			}
		}
		returned.insert(make_pair("action", "GET"));
		returned.insert(make_pair("protocol", domaininfo["protocol"]));
		returned.insert(make_pair("domain", domaininfo["domain"]));
		returned.insert(make_pair("path", domaininfo["path"]));
		returned.insert(make_pair("link", links.dump()));
	}
	return returned;
}

map<string, string> crawl::parse_form(string url) {
	map<string, string> returned;
	json links = json::array();
	HostUtility hostutility;
	EncoderUtility encoder;
	string htmljoin;
	map<string, string> domaininfo = hostutility.domain_info(url);
	map<string, string> res = http_get(url, DEFAULT_HTTP_HEADER);
	if (res["code"] == "200") {
		HtmlUtility html_;
		vector<string> contents = html_.html_array(res["body"]);
		for (string c : contents) {
			if (boost::regex_search(c, boost::regex{"<form|<FORM?|\sname\=\S+?|\sNAME=\S+|<\/form|<\/FORM|<\/ form|</ FORM"})) {
				htmljoin += c;
			}
		}
		vector<string> newhtml;
		boost::split(newhtml, htmljoin, boost::is_any_of("</form|</FORM|</ form|</ FORM|< /form|< /FORM"));
		for (string formtag : newhtml) {
			string action;
			string contenttype = "application/x-www-form-urlencoded";
			string namedata;
			vector<string> names;
			vector<string> values;
			bool isget = false;
			vector<string> formtagsub;
			boost::split(formtagsub, formtag, boost::is_any_of(">"));
			for (string tag : formtagsub) {
				if (boost::regex_search(tag, boost::regex{"\smethod\=\"get\"?|\smethod\=\"GET\"?|\sMETHOD\=\"GET\"?|\smethod\=\'get\'?|\smethod\=\'GET\'?|\sMETHOD\=\'GET\'?|\smethod=GET?|\sMETHOD=GET?|\smethod=get?|\sMETHOD=GET"})) {
					isget = true;
				}
				if (boost::regex_search(tag, boost::regex{"<form?|<FORM"})) {
					string setaction;
					boost::smatch sm;
					boost::regex p{"\saction\=\"(\S+)\"?|\sACTION\=\"(\S+)\"?|\saction\=\'(\S+)\'?|\sACTION\=\'(\S+)\'?|\saction\=(\S+)?|\sACTION\=(\S+)"};
					boost::regex_search(tag, sm, p);
					for (size_t i = 1; i < sm.size(); i++) {
						if (setaction != "") {
							break;
						}
						if (sm[int(i)] != "") {
							setaction = sm[int(i)];
							break;
						}
					}
					if (boost::regex_search(setaction, boost::regex{"^https\:\/\/|^http\:\/\/"})) {
						action = setaction;
					}
					else if (icontains(url, setaction) == false) {
						action = html_.url_filter_extend(url, setaction);
					}
					else {
						action = url;
					}
					string actiontmp = boost::regex_replace(action, boost::regex{"\&amp\;+|\&+"}, "&");
					action = actiontmp;
					if (boost::regex_search(tag, boost::regex{"FORM-DATA?|form-data"})) {
						contenttype = "multipart/form-data";
					}
				}
				else if (boost::regex_search(tag, boost::regex{"\sname\=\"(\S+)\"?|\sNAME\=\"(\S+)\"?|\sname\=\'(\S+)\'?|\sNAME\=\'(\S+)\'?|\sname\=(\S+)?|\sNAME\=(\S+)"})) {
					string setvalue;
					string setname;
					boost::smatch sm;
					boost::regex p;
					boost::regex_search(tag, sm, p);
					for (size_t i = 1; i < sm.size(); i++) {
						if (setname != "") {
							break;
						}
						if (sm[int(i)] != "") {
							setname = sm[int(i)];
							if (find(names.begin(), names.end(), setname) != names.end()) {
								setname = "";
							}
							else {
								names.push_back(setname);
							}
							break;
						}
					}
					if (setname != "") {
						boost::smatch sm;
						boost::regex p {"\svalue\=\"(\S+\s+\S+)\"?|\sVALUE\=\"(\S+\s+\S+)\"?|\svalue\=\'(\S+\s+\S+)\'?|\sVALUE\=\'(\S+\s+\S+)\'?|\svalue\=(\S+\s+\S+)?|\sVALUE\=(\S+\s+\S+)?|\svalue\=\"(\S+)\"?|\sVALUE\=\"(\S+)\"?|\svalue\=\'(\S+)\'?|\sVALUE\=\'(\S+)\'?|\svalue\=(\S+)?|\sVALUE\=(\S+)?|\svalue\=\"(.*?)\"?|\sVALUE\=\"(.*?)\"?|\svalue\=\'(.*?)\'?|\sVALUE\=\'(.*?)\'?|\svalue\=(.*?)?|\sVALUE\=(.*?)"};
						boost::regex_search(tag, sm, p);
						for (size_t i = 1; i < sm.size(); i++) {
							if (setvalue != "") {
								break;
							}
							if (icontains(tag, "value=\"\"")) {
								break;
							}
							else if (icontains(tag, "value=\'\'")) {
								break;
							}
							else if (sm[int(i)] != "") {
								if (boost::regex_search(to_string(sm[int(i)]), boost::regex{"^https\:\/\/?|http\:\/\/"})) {
									string setvaluetmp = boost::regex_replace(to_string(sm[int(i)]), boost::regex{"\s"}, "");
									setvalue = setvaluetmp;
								}
								else {
									string setvaluetmp = boost::regex_replace(to_string(sm[int(i)]), boost::regex{"\s"}, "");
									setvalue = boost::regex_replace(setvaluetmp, boost::regex{"\"+?|\'+?|\/+"}, "");
								}
								break;
							}
						}
						if (setvalue == "") {
							values.push_back("");
						}
						else {
							values.push_back(setvalue);
						}
					}
				}
			}
			if (names.size() > 0) {
				for (int i = 0; i < names.size(); i++) {
					string setvalue = "000";
					if (values[i] != "") {
						if (icontains(values[i], "*") == false) {
							setvalue = values[i];
						}
					}
					if ((i + 1) == names.size()) {
						string tmp = namedata + names[i] + "->" + setvalue;
						namedata = tmp;
					}
					else {
						string tmp = namedata + names[i] + "->" + setvalue + "&";
						namedata = tmp;
					}
				}
				if (action == "") {
					action = url;
				}
				json jsonobj;
				jsonobj.emplace("url", encoder.url_decode(action));
				jsonobj.emplace("data_type", contenttype);
				jsonobj.emplace("data", namedata);
				if (links.contains(to_string(jsonobj)) == false) {
					links.push_back(jsonobj);
				}				
			}
		}
		returned.insert(make_pair("action", "POST"));
		returned.insert(make_pair("protocol", domaininfo["protocol"]));
		returned.insert(make_pair("domain", domaininfo["domain"]));
		returned.insert(make_pair("path", domaininfo["path"]));
		returned.insert(make_pair("link", links.dump()));
	}
	return returned;
}

vector<string> crawl::manage_array(map<string, string> r, vector<string> returned) {
	bool exist = false;
	int position = 0;
	map<string, string> res_;
	for (int i = 0; i < max_deep; i++) {
		json jstringarr = json::array();
		jstringarr = json::parse(r["link"]);
		for (json j : jstringarr) {
			res_ = parse_links(j["url"].get<string>());
			if (returned.size() > 0) {
				json reslinkarray = json::array();
				reslinkarray = json::parse(res_["link"]);
				if (reslinkarray.size() > 0) {					
					for (int i = 0; i < returned.size(); i++) {
						position = i;
						json jsonobj_ = json::parse(returned.at(i));
						json jsonarray_ = json::array();
						jsonarray_ = jsonobj_["link"];
						if (jsonobj_["domain"] == res_["domain"] && jsonobj_["action"] == res_["action"]) {
							exist = true;
							break;
						}
						else {
							exist = false;
						}
					}
					if (exist) {
						json jsonobj_ = json::parse(returned.at(position));
						json linkarray_ = json::array();
						linkarray_ = jsonobj_["link"];
						for (json newurl_ : reslinkarray) {
							if (linkarray_.contains(to_string(newurl_)) == false) {
								linkarray_.push_back(newurl_);
							}
						}
						jsonobj_["link"] = linkarray_;
						returned.at(position) = to_string(jsonobj_);
					}
					else {
						json jsonobj_;
						jsonobj_.emplace("action", res_["action"]);
						jsonobj_.emplace("protocol", res_["protocol"]);
						jsonobj_.emplace("domain", res_["domain"]);
						jsonobj_.emplace("path", res_["path"]);
						jsonobj_.emplace("link", res_["link"]);
						returned.push_back(to_string(jsonobj_));
					}
				}
			}
			else {
				json jsonobj_;
				jsonobj_.emplace("action", res_["action"]);
				jsonobj_.emplace("protocol", res_["protocol"]);
				jsonobj_.emplace("domain", res_["domain"]);
				jsonobj_.emplace("path", res_["path"]);
				jsonobj_.emplace("link", res_["link"]);
				returned.push_back(to_string(jsonobj_));
			}
		}
	}	
	return returned;
}

vector<string> crawl::run(string url) {
	vector<string> returned;
	map<string, string> r;
	if (with_href) {
		r = crawl::parse_links(url);
		json jsonobj_;
		jsonobj_.emplace("action", r["action"]);
		jsonobj_.emplace("protocol", r["protocol"]);
		jsonobj_.emplace("domain", r["domain"]);
		jsonobj_.emplace("path", r["path"]);
		jsonobj_.emplace("link", r["link"]);
		returned.push_back(to_string(jsonobj_));
		returned = crawl::manage_array(r, returned);
	}		
	if (with_form) {
		r = crawl::parse_form(url);
		json jsonobj_;
		jsonobj_.emplace("action", r["action"]);
		jsonobj_.emplace("protocol", r["protocol"]);
		jsonobj_.emplace("domain", r["domain"]);
		jsonobj_.emplace("path", r["path"]);
		jsonobj_.emplace("link", r["link"]);
		returned.push_back(to_string(jsonobj_));
		returned = crawl::manage_array(r, returned);
	}
	return returned;
}
