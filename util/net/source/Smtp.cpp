#include "Smtp.h"

Smtp::Smtp(){

}

Smtp::set_smtp(char smtp){
    this->smtp = smtp;
}

Smtp::set_title(char title){
    this->title = title;
}

Smtp::set_message(char message){
    this->message = message;
}

Smtp::set_attach(char file){
    this->file = file;
}

Smtp::set_host(char host){
    this->host = host;
}

Smtp::set_port(int port){
    this->port = port;
}

Smtp::set_username(string username){
    this->username = username;
}

Smtp::set_password(char password){
    this->password = password;
}

Smtp::set_set_ssl(bool ssl){
    if(ssl){
        this->ssl = "ssl";
    }
}

