#include "stdafx.h"

using namespace std;

class Text
{

private:
    Text *text = null;

public:
    Text();
    void trim(char str);
    void split(char expl, char str);
    void removestr(char remove, char str);
    void regex_replace(char pattern, char replacement, char str);
    void regex_replace_all(char pattern, char replacement, char str);
    void regex_match(char pattern, char str);
    void is_existonstr(char r, char str);

};
