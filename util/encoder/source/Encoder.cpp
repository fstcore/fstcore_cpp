#include "Encoder.h"

string Encoder::utf8_encode(const wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
wstring Encoder::utf8_decode(const string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Convert an wide Unicode string to ANSI string
string Encoder::unicode_to_ansi(const wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an ANSI string to a wide Unicode String
wstring Encoder::ansi_to_unicode(const string& str) {
    int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

string Encoder::md5hash(string e) {
    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(e.data(), e.size());
    hash.get_digest(digest);
    return to_string(digest);
}

string Encoder::randomstring() {
    time_t now = time(0);
    boost::random::mt19937 gen{static_cast<uint32_t>(now)};
    return to_string(gen());
}

string Encoder::base64_decode(string value) {
    string returned;
    boost::beast::detail::base64::decode(&returned, value.data(), value.size());
    return returned;
}

string Encoder::base64_encode(string value) {
    string returned;
    boost::beast::detail::base64::encode(&returned, value.data(), value.size());
    return returned;
}

string Encoder::url_encode(const string& value) {
    static auto hex_digt = "0123456789ABCDEF";

    string result;
    result.reserve(value.size() << 1);

    for (auto ch : value)
    {
        if ((ch >= '0' && ch <= '9')
            || (ch >= 'A' && ch <= 'Z')
            || (ch >= 'a' && ch <= 'z')
            || ch == '-' || ch == '_' || ch == '!'
            || ch == '\'' || ch == '(' || ch == ')'
            || ch == '*' || ch == '~' || ch == '.')  // !'()*-._~
        {
            result.push_back(ch);
        }
        else
        {
            result += string("%") +
                hex_digt[static_cast<unsigned char>(ch) >> 4]
                + hex_digt[static_cast<unsigned char>(ch) & 15];
        }
    }

    return result;
}

string Encoder::url_decode(const string& value) {
    string result;
    result.reserve(value.size());

    for (size_t i = 0; i < value.size(); ++i)
    {
        auto ch = value[i];

        if (ch == '%' && (i + 2) < value.size())
        {
            auto hex = value.substr(i + 1, 2);
            auto dec = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            result.push_back(dec);
            i += 2;
        }
        else if (ch == '+')
        {
            result.push_back(' ');
        }
        else
        {
            result.push_back(ch);
        }
    }

    return result;
}
