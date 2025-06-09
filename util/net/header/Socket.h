#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif

#define DEFAULT_BUFFER_LENGTH 32

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <wincontypes.h>
#include <regex>
#include <iterator>
#include <cstring>  
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <thread>
#include <sys/types.h>
#include <stdlib.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <chrono>
#include <errno.h>
#include <exception>
#include <stdexcept>

/*
void message_callback(SOCKET client_, string data) {
    cout << "message: "+data << endl;
    ofstream save;
    save.open("save.txt", ios::in);
    save << data;
    save.close();
}

void close_callback(SOCKET client_) {
    //socket_.close(client_);
    cout << "closed "<< endl;
}

void open_callback(SOCKET client_) {
    socket_.write(client_, "test");
}

void error_callback(SOCKET client_, int error) {
    string printdata = "error: " + to_string(error);
    cout << printdata << endl;
}
*/

using namespace std;

class SocketUtility {
    typedef void(*on_message)(SOCKET, string);
    typedef void(*on_open)(SOCKET);
    typedef void(*on_close)(SOCKET);
    typedef void(*on_error)(SOCKET, int);

public:
    map<string, map<string, string>> status;
    sockaddr_in sock_service;
    SOCKET ConnectSocket = INVALID_SOCKET, ListenSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int byteConn, byteRecv, byteSent, byteListen;
    int recvbuflen = DEFAULT_BUFFER_LENGTH;
    on_message msg_callback;
    on_open open_callback;
    on_close close_callback;
    on_error error_callback;

public:
    void callback(on_open open, on_close close, on_message msg, on_error err);
    void loop_client_(SOCKET client_);
    void accept_(SOCKET server_);
    SOCKET listen_(string ip, int port);
    SOCKET open_(string ip, int port);
    bool write(SOCKET client_, string data);
    string receive(SOCKET client_);
    string client_info(SOCKET client_);
    void close(SOCKET client_);
    vector<string> get_ip(string hostname);
};
