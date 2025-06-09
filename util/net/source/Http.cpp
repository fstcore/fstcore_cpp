#include "HttpRequest.h"

typedef enum Http::proxy_type;
typedef enum Http::post_type;
typedef enum Http::request_type;
string Http::url;
	string Http::header;
	string Http::cookie;
	int Http::timeout = 25000;
	string Http::postdata;
	string Http::cookiefile;
	char Http::response_uri = NULL;
	long Http::response_code;
	string Http::response_header;
	string Http::response_body;
	map<string, string> Http::response;
	gcroot<ConsoleLog^> Http::log = gcnew ConsoleLog();
	CURL *Http::curl;
	CURLcode Http::res;
	struct curl_slist *Http::slist = NULL;
	struct curl_httppost *Http::formpost = NULL;
	struct curl_httppost *Http::lastptr = NULL;
	CURL *Http::curl_handle = curl_easy_init();
	map<string, proxy_type> Http::enum_proxy;
	map<string, post_type> Http::enum_post;
	map<string, request_type> Http::enum_request;

HttpRequest::HttpRequest() {
	this->enum_proxy["http"] = this->http;
	this->enum_proxy["http"] = this->http;
	this->enum_proxy["http"] = this->http;
	this->enum_proxy["https"] = this->https;
	this->enum_proxy["socks5"] = this->socks5;
	this->enum_proxy["socks4"] = this->socks4;
	this->enum_proxy["socks4a"] = this->socks4a;
	this->enum_post["raw"] = this->raw;
	this->enum_post["data"] = this->data;
	this->enum_post["form"] = this->form;
	this->enum_request["GET"] = this->GET;
	this->enum_request["POST"] = this->POST;
	this->enum_request["PUT"] = this->PUT;
	this->enum_request["PROFIND"] = this->PROFIND;
	this->enum_request["HEAD"] = this->HEAD;
	this->enum_request["DEL"] = this->DEL;
	string x = "./" + md5hash(randomstring()) + ".txt";
	this->cookiefile = x;
}

size_t HttpRequest::header_callback(char* buffer, size_t size, size_t nitems, void* userdata)
{
	string* headers = (string*)userdata;
	headers->append(buffer, (size_t)(nitems * size));
	return (size_t)(nitems * size);
}

size_t HttpRequest::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return (size_t)(size * nmemb);
}

string HttpRequest::md5hash(string e) {
	md5 hash;
	md5::digest_type digest;
	hash.process_bytes(e.data(), e.size());
	hash.get_digest(digest);
	return to_string(digest);
}

string HttpRequest::randomstring() {
	time_t now = time(0);
	boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
	return to_string(gen());
}

void HttpRequest::set_url(string url) {
	this->url = url;
	//curl_global_init(CURL_GLOBAL_ALL);
	this->curl = curl_easy_init();
	curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
	//curl_easy_setopt(this->curl, CURLOPT_CAPATH, "C:/BurpsuiteCertificate/");
	//curl_easy_setopt(this->curl, CURLOPT_SSLCERT, "C:/BurpsuiteCertificate/cacert.der");
	//curl_easy_setopt(this->curl, CURLOPT_SSLCERTTYPE, "DER");

	/*curl_easy_setopt(this->curl, CURLOPT_SSLKEY, "C:/BurpsuiteCertificate/client_key.pem");
	curl_easy_setopt(this->curl, CURLOPT_SSLKEYTYPE, "PEM");
	curl_easy_setopt(this->curl, CURLOPT_CAINFO, "C:/BurpsuiteCertificate/ca_info.pem");
	curl_easy_setopt(this->curl, CURLOPT_KEYPASSWD, "123");*/

	//curl_easy_setopt(this->curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(this->curl, CURLOPT_COOKIEFILE, this->cookiefile);
	curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->curl, CURLOPT_COOKIEJAR, this->cookiefile);
	curl_easy_setopt(this->curl, CURLOPT_HEADERFUNCTION, &this->header_callback);
	curl_easy_setopt(this->curl, CURLOPT_HEADERDATA, &this->response_header);
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &this->WriteCallback);
	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &this->response_body);
	//curl_easy_setopt(this->curl, CURLOPT_TIMEOUT_MS, this->timeout);
}

void HttpRequest::set_timeout(int t) {
	this->timeout = t;
	curl_easy_setopt(this->curl, CURLOPT_TIMEOUT_MS, (long)t);
}

void HttpRequest::add_header(string h) {
	if (icontains(h, "&")) {
		vector<string> res;
		boost::split(res, h, boost::is_any_of("\\&"));
		for (string e : res) {
			string s = regex_replace(e, std::regex("\\->"), ": ");
			this->slist = curl_slist_append(this->slist, s.c_str());
		}
	}
	else {
		string s = std::regex_replace(h, std::regex("\\->"), ": ");
		this->slist = curl_slist_append(this->slist, s.c_str());
	}
	curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, this->slist);
}

void HttpRequest::add_cookie(string c) {
	string tmp = regex_replace(c, std::regex("cookie\\:|Cookie\\s+\\:"), ""); //regex_replace(c, regex("cookie\\:?|Cookie\\s+\\:"), "");
	string s = "Cookie: " + tmp;
	this->slist = curl_slist_append(this->slist, (const char*)s.c_str());
}

void HttpRequest::set_proxy(string type, string ip, int port, char auth) {
	string proxy = type+"://"+ip+":"+ to_string(port);
	switch (this->enum_proxy[type]) {
	case this->http:
		curl_easy_setopt(this->curl, CURLOPT_PROXY, proxy.c_str());
		break;
	case this->https:
		curl_easy_setopt(this->curl, CURLOPT_PROXY, proxy.c_str());
		break;
	case this->socks5:
		curl_easy_setopt(this->curl, CURLOPT_PROXY, proxy.c_str());
		break;
	case this->socks4:
		curl_easy_setopt(this->curl, CURLOPT_PROXY, proxy.c_str());
		break;
	case this->socks4a:
		curl_easy_setopt(this->curl, CURLOPT_PROXY, proxy.c_str());
		break;
	default:
		break;
	}
	if (auth != NULL) {
		curl_easy_setopt(this->curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
		curl_easy_setopt(this->curl, CURLOPT_PROXYUSERPWD, auth);
	}
}

void HttpRequest::set_postdata(string type, string d) {
	switch (this->enum_post[type]) {
	case this->raw:
		this->postdata = d;
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, this->postdata.c_str());
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDSIZE, this->postdata.length());
		break;
	case this->data:
		this->postdata = regex_replace(d, std::regex("\\->"), "=");
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, this->postdata.c_str());
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDSIZE, this->postdata.length());
		break;
	case this->form:
		curl_httppost *formpostt;
		curl_httppost *lastptrr;
		if (icontains(d, "&")) {
			vector<string> res;
			boost::split(res, d, boost::is_any_of("\\&"));
			for (string e : res) {
				vector<string> tmp;
				boost::split(tmp, e, boost::is_any_of("->"));
				curl_formadd(&formpostt,
					&lastptrr,
					CURLFORM_COPYNAME, tmp.at(0),
					CURLFORM_FILE, tmp.at(1),
					CURLFORM_END);
			}
		}
		else {
			vector<string> tmp;
			boost::split(tmp, d, boost::is_any_of("->"));
			curl_formadd(&formpostt,
				&lastptrr,
				CURLFORM_COPYNAME, tmp.at(0),
				CURLFORM_FILE, tmp.at(1),
				CURLFORM_END);
		}
		this->formpost = formpostt;
		this->lastptr = lastptrr;
		curl_easy_setopt(this->curl, CURLOPT_HTTPPOST, this->formpost);
		break;
	default:
		break;
	}
}

string HttpRequest::get_cookies() {
	/*ifstream x(*this->cookiefile);
	string d = to_string(x.);
	return d;*/
	return "s";
}

map<string, string> HttpRequest::request(string type) {
	switch (this->enum_request[type]) {
	case this->GET:
		this->res = curl_easy_perform(this->curl);
		break;
	case this->POST:
		curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(this->curl, CURLOPT_POST, 1);
		this->res = curl_easy_perform(this->curl);
		break;
	case this->PUT:
		curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(this->curl, CURLOPT_PUT, 1L);
		this->res = curl_easy_perform(this->curl);
		break;
	case this->PROFIND:
		curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "PROFIND");
		this->res = curl_easy_perform(this->curl);
		break;
	case this->HEAD:
		curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "HEAD");
		this->res = curl_easy_perform(this->curl);
		break;
	case this->DEL:
		curl_easy_setopt(this->curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		this->res = curl_easy_perform(this->curl);
		break;
	default:
		break;
	}
	curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &response_code);
	curl_easy_getinfo(this->curl, CURLINFO_EFFECTIVE_URL, &response_uri);
	if (this->res != CURLE_OK) {
		this->log->alert("error", curl_easy_strerror(res));
		this->response.insert(make_pair("code", to_string(this->response_code)));
		this->response.insert(make_pair("uri", to_string(this->response_uri)));
		this->response.insert(make_pair("header", this->response_header));
		this->response.insert(make_pair("cookie", ""));
		this->response.insert(make_pair("body", this->response_body));
	}
	else {
		this->response.insert(make_pair("code", to_string(this->response_code)));
		this->response.insert(make_pair("uri", to_string(this->response_uri)));
		this->response.insert(make_pair("header", this->response_header));
		this->response.insert(make_pair("cookie", ""));
		this->response.insert(make_pair("body", this->response_body));
	}
	curl_easy_cleanup(this->curl);
	curl_slist_free_all(this->slist);
	if (FILE* file = fopen(this->cookiefile.c_str(), "r")) {
		fclose(file);
		_unlink(this->cookiefile.c_str());
	}
	return this->response;
}

