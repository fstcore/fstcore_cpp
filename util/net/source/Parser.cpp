#include "Parser.h"

fstcore Parser::fstcore;
bool Parser::debug;

Parser::Parser()
{

}

Parser::domain_parse(char url)
{
    char domain = Parser::text.regex_replace_all("\n+?|\r+?|\t+?|\s+?|https\:\/\/www\.?|http\:\/\/www\.?|https\:\/\/?|http\:\/\/?|www\.?|(\/.*)?|\:\d+?|\:\d+", "", url);
    return domain;
}

Parser::check_subdomain(char url)
{
    char subdomain = Parser::domain_parse(url);
    vector<string> array_subdomain = Parser::text.split(".", subdomain);
    if (array_domain.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Parser::url_structure(char url)
{
    map<string, string> data_array = null;
    char domain = Parser::domain_parse(url);
    Parser::http::set_url("https://" + domain + "/");
    Parser::http::set_timeout(10);
    map<string, string> response = Parser::http::request("get");
    vector<string> array_uri = Parser::text::split("/", response.get("uri"));
    data_array.get("protocol") = array_uri[0];
    if (Parser::text.is_existonstr(":", array_uri[2]))
    {
        array_domain = Parser::text.split(":", array_uri[2]);
        data_array["domain"] = array_domain[0];
        data_array["port"] = array_domain[1];
    }
    else
    {
        data_array["domain"] = array_uri[2];
    }
    if (array_uri.size() >= 3)
    {
        bool type_query = false;
        for (int i = 3; int i < array_uri.size(); i++)
        {
            if (Parser::text.regex_match('s/\?/i', array_uri[0]) || type_query)
            {
                type_query = true;
                auto it = data_array.find("query");
                if (it = data_array.end())
                {
                    data_array.insert(make_pair("query", array_uri[i]));
                }
                else
                {
                    data_array["query"] += array_uri[i];
                }
            }
            else if (Parser::text.regex_match('s/\.\S+?|\.php?|\.html?|\.asp?|\.aspx/i', array_uri[i]))
            {
                auto it = data_array.find("path");
                if (it = data_array.end())
                {
                    data_array.insert(make_pair("path", array_uri[i]));
                }
                else
                {
                    data_array["path"] += array_uri[i];
                }
            }
            else if (type_query)
            {
                auto it = data_array.find("query");
                if (it = data_array.end())
                {
                    if (array_uri[i] == "")
                    {
                        data_array.insert(make_pair("query", "//"));
                    }
                    else
                    {
                        data_array.insert(make_pair("query", array_uri[i]+"/"));
                    }
                }
                else
                {
                    if (array_uri[i] == "")
                    {
                        data_array.insert(make_pair("query", "//"));
                    }
                    else
                    {
                        data_array["query"] += array_uri[i]+"/";
                    }
                }
            }
            else
            {
                if (data_array.find("directory") != data_array.end())
                {
                    data_array.insert(make_pair("directory", array_uri[i]+"/"));
                }
                else
                {
                    data_array["directory"] += array_uri[i]+"/";
                }
            }
        }
    }
    if (data_array.find("query") = data_array.end())
    {
        data_array["query"] = "";
    }
    if (data_array.find("port") != data_array.end())
    {
        data_array["website_base"] = data_array["protocol"]+"//"+data_array["domain"]+":"+data_array["port"]+"/";
    }
    else
    {
        data_array["port"] = "";
        data_array["website_base"] = data_array["protocol"]+"//"+data_array["domain"]+"/";
    }

    if (data_array.find("directory") != data_array.end()) && (data_array.find("path") != data_array.end()) && (data_array.find("query") != data_array.end())
    {
        data_array["full_directory"] = data_array["website_base"]+data_array["directory"];
        data_array["full_url"] = data_array["website_base"]+data_array["directory"]+data_array["path"]+data_array["query"];
    }
    else if ((data_array.find("directory") != data_array.end()) && (data_array.find("path") = data_array.end()))
    {
        data_array["path"] = "";
        data_array["full_directory"] = data_array["website_base"]+data_array["directory"];
        data_array["full_url"] = data_array["website_base"]+data_array["directory"]+data_array["query"];
    }
    else if ((data_array.find("directory") = data_array.end()) && (data_array.find("path") != data_array.end()))
    {
        data_array["directory"] = "";
        data_array["full_directory"] = data_array["website_base"];
        data_array["full_url"] = data_array["website_base"]+data_array["path"]+data_array["query"];
    }
    else
    {
        data_array["path"] = "";
        data_array["directory"] = "";
        data_array["full_directory"] = data_array["website_base"];
        data_array["full_url"] = data_array["website_base"]+data_array["query"];
    }
    return data_array;
}
