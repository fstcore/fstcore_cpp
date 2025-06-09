#include "SocketUtility.h"

void SocketUtility::loop_client_(SOCKET client_) {
    string ip = client_info(client_);
    if (client_ == SOCKET_ERROR) {
        int err = WSAGetLastError();
        error_callback(client_, err);
    }
    else {
        open_callback(client_);
    }
    while (1) {
        string data = receive(client_);
        if (status[ip]["read"] == "error" || status[ip]["write"] == "error") {
            shutdown(client_, SD_BOTH);
            status.erase(ip);
            close_callback(client_);
            break;
        }
        else {
            msg_callback(client_, data);
        }
    }
}

void SocketUtility::callback(on_open open, on_close close, on_message msg, on_error err) {
    open_callback = open;
    close_callback = close;
    msg_callback = msg;
    error_callback = err;
}

string SocketUtility::client_info(SOCKET client_) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(client_, (SOCKADDR*)&addr, &addr_size);
    char ip[100];
    int port = ntohs(addr.sin_port);
    strcpy_s(ip, inet_ntoa(addr.sin_addr));
    string ipaddress = ip;
    string returned = ipaddress + ":" + to_string(port);
    return returned;
}

void SocketUtility::accept_(SOCKET server_) {
    SOCKET client_;
    while (1) {
        client_ = SOCKET_ERROR;
        while (client_ == SOCKET_ERROR) {
            client_ = accept(ListenSocket, NULL, NULL);
        }
        thread t([this, client_ = move(client_)] {
            loop_client_(client_);
        });
        t.detach();
        client_ = SOCKET_ERROR;
    }
}

SOCKET SocketUtility::listen_(string ip, int port) {
    byteListen = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (byteListen != 0) {
        //printf("WSAStartup failed with error: %d\n", byteConn);
        return SOCKET_ERROR;
    }
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        //printf("create socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return SOCKET_ERROR;
    }
    vector<string> ips = get_ip(ip);
    ZeroMemory(&sock_service, sizeof(sock_service));
    sock_service.sin_family = AF_INET;
    sock_service.sin_addr.S_un.S_addr = inet_addr(ips.at(0).c_str());
    sock_service.sin_port = htons(port);
    byteListen = bind(ListenSocket, (SOCKADDR*)&sock_service, sizeof(sock_service));
    if (byteListen == SOCKET_ERROR) {
        close(ListenSocket);
        return SOCKET_ERROR;
    }
    byteListen = listen(ListenSocket, SOMAXCONN);
    if (byteListen == SOCKET_ERROR) {
        close(ListenSocket);
        return SOCKET_ERROR;
    }
    return ListenSocket;
}

SOCKET SocketUtility::open_(string ip, int port) {
    byteConn = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (byteConn != 0) {
        //printf("WSAStartup failed with error: %d\n", byteConn);
        return SOCKET_ERROR;
    }
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        //printf("create socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return SOCKET_ERROR;
    }
    vector<string> ips = get_ip(ip);
    ZeroMemory(&sock_service, sizeof(sock_service));
    sock_service.sin_family = AF_INET;
    sock_service.sin_addr.S_un.S_addr = inet_addr(ips.at(0).c_str());
    sock_service.sin_port = htons(port);
    if (connect(ConnectSocket, (SOCKADDR*)&sock_service, sizeof(sock_service)) == SOCKET_ERROR) {
        close(ConnectSocket);
        return SOCKET_ERROR;
    }
    return ConnectSocket;
}

bool SocketUtility::write(SOCKET client_, string data) {
    bool returned = false;
    int state;
    string ip = client_info(client_);
    const char* sendbuf = data.c_str();
    state = send(client_, sendbuf, strlen(sendbuf), 0);
    if (status.count(ip)) {
        map<string, string> states = status[ip];
        if (state == SOCKET_ERROR) {
            states["read"] = "error";
            returned = false;
        }
        else {
            states["read"] = "alive";
            returned = true;
        }
        status[ip] = states;
    }
    else {
        map<string, string> states;
        if (state == SOCKET_ERROR) {
            states["read"] = "error";
            returned = false;
        }
        else {
            states["read"] = "alive";
            returned = true;
        }
        status.insert(make_pair(ip, states));
    }
    return returned;
}

string SocketUtility::receive(SOCKET client_) {
    string returned;
    char recvbuf[DEFAULT_BUFFER_LENGTH];
    string ip = client_info(client_);
    int state;
    //do {
        struct sockaddr_in SenderAddr;
        int SenderAddrSize = sizeof(SenderAddr);
        state = recvfrom(client_, recvbuf, recvbuflen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
        returned = recvbuf;
        if (status.count(ip)) {
            map<string, string> states = status[ip];
            if (state == SOCKET_ERROR) {
                states["read"] = "error";
            }
            else {
                states["read"] = "alive";
            }
            status[ip] = states;
        }
        else {
            map<string, string> states;
            if (state == SOCKET_ERROR) {
                states["read"] = "error";
            }
            else {
                states["read"] = "alive";
            }
            status.insert(make_pair(ip, states));
        }
        //byteRecv = 0;
    //} while (byteRecv > 0);
    return returned;
}

void SocketUtility::close(SOCKET client_) {
    closesocket(client_);
    WSACleanup();
}

vector<string> SocketUtility::get_ip(string hostname) {
    vector<string> returned;
    struct hostent* remoteHost = gethostbyname(hostname.c_str());
    //char* host_name;
    struct in_addr addr;
    int i = 0;
    while (remoteHost->h_addr_list[i] != 0) {
        addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
        returned.push_back(inet_ntoa(addr));
        //printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
    }
    return returned;
}
