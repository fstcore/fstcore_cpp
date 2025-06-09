#include "pch.h"
#include "Host.h"

string Host::proxy_type;
string Host::proxy_host;
int Host::proxy_port;
string Host::proxy_username;
string Host::proxy_password;

Host::Host() {
}

map<string, string> Host::http_get(string url, string header) {
	HttpRequestCurl http;
	http.set_url(url);
	http.set_timeout(30000);
	http.add_header(header);
	if (proxy_type.length() > 0 && proxy_username.length() > 0) {
		string proxyauth = to_string(proxy_username + ":" + proxy_password);
		http.set_proxy(proxy_type, proxy_host, proxy_port, (char) proxyauth.c_str());
	}
	else if (proxy_type.length() > 0) {
		http.set_proxy(proxy_type, proxy_host, proxy_port, NULL);
	}
	return http.request("GET");
}

map<string, string> Host::http_post(string url, string header, string data, string d_type) {
	HttpRequestCurl http;
	http.set_url(url);
	http.set_timeout(30000);
	http.add_header(header);
	if (proxy_type.length() > 0 && proxy_username.length()> 0) {
		string proxyauth = to_string(proxy_username + ":" + proxy_password);
		http.set_proxy(proxy_type, proxy_host, proxy_port, (char) proxyauth.c_str());
	}
	else if (proxy_type.length() > 0) {
		http.set_proxy(proxy_type, proxy_host, proxy_port, NULL);
	}
	http.set_postdata(d_type, data);
	return http.request("POST");
}

map<string, string> Host::host_info(string host) {
	map<string, string> returned;
	map<string, string> res;
	res = http_get("http://ip-api.com/json/" + host, DEFAULT_HTTP_HEADER);
	json jsonobj = json::parse(res["body"]);
	returned.emplace("ip", jsonobj["ip"]);
	returned.emplace("country_code", jsonobj["countryCode"]);
	returned.emplace("country_name", jsonobj["country"]);
	returned.emplace("region_code", jsonobj["region"]);
	returned.emplace("region_name", jsonobj["regionName"]);
	returned.emplace("isp", jsonobj["ips"]);
	returned.emplace("timezone", jsonobj["timezone"]);
	return returned;
}

string Host::netmask(string host) {
	map<string, string> res;
	res = http_get("https://check-host.net/ip-info?host=" + host, DEFAULT_HTTP_HEADER);
	boost::regex p{ "(\\d+\\.\\d+\\.\\d+\\.\\d+\\-\\d+\\.\\d+\\.\\d+\\.\\d+)" };
	boost::smatch m;
	boost::regex_search(res["body"], m, p);
	return m[int(4)];
}

vector<string> Host::generate_ip(string host) {
	vector<string> returned;
	bool stop = false;
	string netmask_ = netmask(host);
	vector<string> netmask_split;
	boost::split(netmask_split, netmask_, boost::is_any_of("-"));
	string start = netmask_split.at(0);
	string end = netmask_split.at(1);
	vector<string> x;
	boost::split(x, start.c_str(), boost::is_any_of("."));
	for (int i = int(x[0].c_str()); i < 257; i++) {
		if (stop) {
			break;
		}
		for (int j = int(x[1].c_str()); j < 257; j++) {
			if (j == 256) {
				j = 1;
			}
			for (int k = int(x[2].c_str()); k < 257; k++) {
				if (stop) {
					break;
				}
				if (k == 256) {
					k = 1;
				}
				for (int l = int(x[3].c_str()); l < 256; l++) {
					if (stop) {
						break;
					}
					if (l == 256) {
						l = 1;
					}
					else {
						string newip = to_string(i) + "." + to_string(j) + "." + to_string(k) + "." + to_string(l);
						returned.push_back(newip);
						if (newip == end) {
							stop = true;
							break;
						}
					}
				}
			}
		}
	}
	return returned;
}

bool Host::check_port(string host, int port) {
	SocketUtility socket;
	if (proxy_type.length() > 0) {
		SOCKET stat = socket.open_(proxy_host, proxy_port);
		if (stat != SOCKET_ERROR) {
			if (proxy_username.length() > 0) {
				string data = "CONNECT " + host + ":" + to_string(port) + " HTTP/1.1\r\nHost: " + host + ":" + to_string(port) + "\r\nProxy-Authenticate: Basic " + proxy_username + " : " + proxy_password + "\r\nAccept: plain/text\r\n\r\n\r\n\r\n";
				socket.write(stat, data);
			} else {
				string data = "CONNECT " + host + ":" + to_string(port) + " HTTP/1.1\r\nHost: " + host + ":" + to_string(port) + "\r\nAccept: plain/text\r\n\r\n\r\n\r\n";
				socket.write(stat, data);
			}
			if (sizeof(socket.receive(stat)) > 0) {
				return true;
			} else {
				return false;
			}
		} else {
			socket.close(stat);
			return false;
		}
	}
	else {
		bool stat = socket.open_(host, port);
		if (stat != SOCKET_ERROR) {
			string data = "CONNECT " + host + ":" + to_string(port) + " HTTP/1.1\r\nHost: " + host + ":" + to_string(port) + "\r\nAccept: plain/text\r\n\r\n\r\n\r\n";
			socket.write(stat, data);
			if (sizeof(socket.receive(stat)) > 0) {
				return true;
			} else {
				return false;
			}			
		} else {
			socket.close(stat);
			return false;
		}
	}
}

map<string, string> Host::domain_info(string host) {
	map<string, string> returned;
	HtmlUtility html_;
	string domain = html_.clear_domain(host);
	string url = "https://www." + domain + "/";
	map<string, string> res;
	res = http_get(url, DEFAULT_HTTP_HEADER);
	vector<string> domain_split;
	boost::split(domain_split, res["uri"], boost::is_any_of("/"));
	string protocol = domain_split.at(0) + "//";
	domain = domain_split.at(2);
	string path = domain_split.at(3);
	returned.insert(make_pair("protocol", protocol));
	returned.insert(make_pair("domain", domain));
	returned.insert(make_pair("path", path));
	return returned;
}

vector<string> Host::get_ip(string hostname) {
	vector<string> returned;
	struct hostent* remoteHost = gethostbyname(hostname.c_str());
	//char* host_name;
	struct in_addr addr;
	int i = 0;
	while (remoteHost->h_addr_list[i] != 0) {
		addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
		returned.push_back(inet_ntoa(addr));
		//printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
	}
	return returned;
}
