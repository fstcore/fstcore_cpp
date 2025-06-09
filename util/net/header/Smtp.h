#include "stdafx.h"

class Smtp{

    private:
        FileSystem fs = null;
        Smtp smtp_json = null;
        char smtp = null;
        char host = null;
        char port = null;
        char username = null;
        char password = null;
        char from = null;
        char to = null;
        char ssl = null;
        char title = null;
        char message = null;
        char file = null;

    public:
        void set_smtp(char smtp);
        void set_title(char title);
        void set_message(char message);
        void set_attach(char file);
        void set_host(char host);
        void set_port(char port);
        void set_username(char username);
        void set_password(char password);
        void set_ssl(char ssl);
        void send();

};
