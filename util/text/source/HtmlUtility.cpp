#include "pch.h"
#include "HtmlUtility.h"

HtmlUtility::HtmlUtility() {
}

vector<string> HtmlUtility::html_array(string html) {
	vector<string> returned;
	string html1 = boost::regex_replace(html, boost::regex{"(\n+|\t+|\r+)"}, "");
	string html2 = boost::regex_replace(html1, boost::regex{"(><+)"}, ">\n<");
	//?|<IFRAME.*\n+?|<iframe.*\n+?|<style.*\n+?|<link.*\n+?|<LINK.*\n+?|<meta.*\n+?|<META.*\n+
	string html3 = boost::regex_replace(html2, boost::regex{"(<script.*\n+|<SCRIPT.*\n+)"}, "");
	string html4 = boost::regex_replace(html3, boost::regex{"(>+)"}, ">\n");
	string html5 = boost::regex_replace(html4, boost::regex{"\&amp\;+|\&+"}, "&");
	boost::split(returned, html5, boost::is_any_of(">"));
	return returned;
}


string HtmlUtility::url_filter_extend(string url, string link) {
	string returned;
	HostUtility hostutility;
	HtmlUtility htmlutility;
	map<string, string> domaininfo = hostutility.domain_info(url);
	string protocol = domaininfo["protocol"];
	if (boost::regex_search(link, boost::regex{"^\/"})) {
		string tmp = boost::regex_replace(link, boost::regex{"^\/"}, "");
		if (icontains(tmp, domaininfo["domain"]) == false) {
			returned = protocol + domaininfo["domain"] + "/" + tmp;
		}
		else {
			returned = protocol + "" + tmp;
		}
	}
	else if (boost::regex_search(link, boost::regex{"^\.\/"})) {
		string uritmp = boost::regex_replace(url, boost::regex{"\w+\.php$|\w+\.php?.*|\w+\.asp$|\w+\.asp?.*|\w+\.aspx$|\w+\.aspx?.*|mailto\:.*"}, "");
		string uritmp2 = htmlutility.arrange_slash(uritmp);
		string uritmp3 = boost::regex_replace(link, boost::regex{"\.\/|^\."}, "");
		returned = uritmp2 + uritmp3;
	}
	else if (boost::regex_search(link, boost::regex{"\.\?"})) {
		string uritmp = boost::regex_replace(url, boost::regex{"\w+\.php$|\w+\.php?.*|\w+\.asp$|\w+\.asp?.*|\w+\.aspx$|\w+\.aspx?.*|mailto\:.*"}, "");
		string uritmp2 = htmlutility.arrange_slash(uritmp);
		string uritmp3 = boost::regex_replace(link, boost::regex{"\.\/|^\."}, "");
		returned = uritmp2 + uritmp3;
	}
	else if (boost::regex_search(link, boost::regex{"^\.\."})) {
		string dir = boost::regex_replace(protocol, boost::regex{"\/"}, url);
		dir = htmlutility.arrange_slash(dir);
		string newuriset = "";
		int count = 0;
		boost::smatch sm;
		boost::regex p{"(\.\.)"};
		boost::regex_search(link, sm, p);
		for (size_t i = 1; i < sm.size(); i++) {
			if (sm[int(i)] != "") {
				count += 1;
			}
		}
		if (count > 0) {
			vector<string> newuris;
			boost::split(newuris, dir, boost::is_any_of("\/"));
			int countselect = sizeof(newuris) - count - 1;
			for (int i = 0; i < countselect; i++) {
				if (icontains(newuris[i], ".php") == false) {
					string tmp = newuriset + newuris[i];
					newuriset = tmp;
				}
			}
		}
		else {
			newuriset = dir;
		}
		string newpath = boost::regex_replace(link, boost::regex{"\.\.\/"}, "/");
		string pathset = newuriset + newpath;
		string newpathx = htmlutility.arrange_slash(pathset);
		returned = protocol + newpathx;
	}
	else {
		url = boost::regex_replace(url, boost::regex{"\w+\.php$|\w+\.php?.*|\w+\.asp$|\w+\.asp?.*|\w+\.aspx$|\w+\.aspx?.*|mailto\:.*"}, "");
		url = htmlutility.arrange_slash(url);
		returned = url + link;
	}
	return returned;
}

bool HtmlUtility::filter_extension(string e) {
	boost::regex image_ext{"\.ico|\.jpg|\.jpeg|\.png|\.bmp"};
	boost::regex file_ext{"\.exe|\.EXE|\.txt|\.zip|\.rar|\.iso|\.gz"};
	boost::regex media_ext{"\.mkv|\.mp3|\.mp4"};
	if ((boost::regex_search(e, image_ext) == false) && (boost::regex_search(e, file_ext) == false) && (boost::regex_search(e, media_ext) == false)) {
		return false;
	}
	else {
		return true;
	}
}

string HtmlUtility::url_filter(string url, string link) {
	HtmlUtility htmlutility;
	if (boost::regex_search(link, boost::regex{"^http|^https"})) {
		return link;
	}
	else if (icontains(link, ".php") && icontains(link, "&")) {
		return htmlutility.url_filter_extend(url, link);
	}
	else if (icontains(link, "&")) {
		return htmlutility.url_filter_extend(url, "index.php" + link);
	}
	else {
		return htmlutility.url_filter_extend(url, link);
	}
}

string HtmlUtility::clear_domain(string host) {
    string domain = boost::regex_replace(host, boost::regex{"^(https:\\/\\/www\\.?|http:\\/\\/www\\.?|https:\\/\\/?|http:\\/\\/?|www\\.)?|(\\/.*)$"}, "");
    return domain;
}

string HtmlUtility::arrange_slash(string url) {
    string returned;
    vector<string> paths;
    boost::split(paths, url, boost::is_any_of("/"));
    for (string p : paths) {
        if ((p == "") && (returned != "")) {
            returned += "/" + p;
        }
        else if ((p != "") && (returned == "")) {
            returned += p;
        }
    }
    if (boost::regex_search(returned, boost::regex{"\.php$|\.php\?|\.asp$|\.asp\?|\.aspx$|\.aspx\?"}) == false) {
        returned = returned + "/";
    }
    if (boost::regex_search(returned, boost::regex{"https"})) {
        returned = boost::regex_replace(returned, boost::regex{"https\:\/"}, "https://");
    }
    else if (boost::regex_search(returned, boost::regex{"http"})) {
        returned = boost::regex_replace(returned, boost::regex{"http\:\/"}, "http://");
    }
    return returned;
}

