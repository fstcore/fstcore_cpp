using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;

class HostUtility {

public:
	fstcore fstcore;
    bool debug;
	static string proxy_type;
	static string proxy_host;
	static int proxy_port;
	static string proxy_username;
	static string proxy_password;

private:
	map<string, string> http_get(string url, string header);
	map<string, string> http_post(string url, string header, string data, string d_type);

public:
	HostUtility();
	map<string, string> host_info(string host);
	string netmask(string host);
	vector<string> generate_ip(string host);
	bool check_port(string host, int port);
	map<string, string> domain_info(string host);
	vector<string> get_ip(string hostname);
};
