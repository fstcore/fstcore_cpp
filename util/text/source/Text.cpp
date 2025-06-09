#include "Text.h"

Text::Text(){

}

Text::trim(char str){
    regex pattern("\s+?|\t+?|\r+?|\n+");
    return regex_replace(str, pattern, "", regex_constants::match_any);
}

Text::split(char expl, char str){
    regex pattern(expl);
    sregex_token_iterator iter(input.begin(), input.end(), pattern, -1);
    sregex_token_iterator end;
    vector<std::string> result(iter, end);
    return result;
}

Text::removestr(char remove, char str){
    size_t pos = str.find(remove);
    if(pos != string::npos){
        str.erase(pos, remove.length());
    }
    return str;
}

Text::regex_replace(char pattern, char replacement, char str){
    regex pattern(pattern);
    return regex_replace(str, pattern, replacement);
}

Text::regex_replace_all(char pattern, char replacement, char str){
    regex pattern(pattern);
    return regex_replace(str, pattern, replacement, regex_constants::match_any);
}

Text::regex_match(char pattern, char str){
    regex pattern(pattern);
    if(regex_match(str, pattern)){
        return true;
    }else{
        return false;
    }
}

Text::is_existonstr(char r, char str){
    if(str.find(r) != string::npos){
        return true;
    }else{
        return false;
    }
}
