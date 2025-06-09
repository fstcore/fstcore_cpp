#include "stdafx.h"

class Parser{

    private:
        fstcore fstcore;
        bool debug;

    public:
        Parser();
        void domain_parse(char url);
        void check_subdomain(char url);
        void url_structure(char url);

};
