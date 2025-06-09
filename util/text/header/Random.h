#include "stdafx.h"

using namespace std;

class Random{

    public:
    Random();
    char random(vector<string> array, size_t length);
    int random_number(size_t length);
    char random_char_lower(size_t length);
    char random_char_upper(size_t length);
    char random_symbol(size_t length);
    char password(char type=null, size_t, length);

};
