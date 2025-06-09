#include "pch.h"

using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;

SocketUtility socket_;
string test = "test";

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

int main(int argc, char* argv[]) {
    crawl h;
    h.with_href = true;
    h.max_deep = 3;
    vector<string> r = h.run("https://stackoverflow.com/");
    for (string x : r) {
        json r = json::parse(x);
        json link = json::array();
        link = json::parse(r["link"].get<string>());
        if (link.size() > 0) {
            for (json d : link) {
                cout << d["url"].get<string>() << endl;
            }
        }
    }
    /*SOCKET server_;
    if ((server_ = socket_.listen_("0.0.0.0", 5555)) != SOCKET_ERROR) {
        socket_.callback(&open_callback, &close_callback, &message_callback, &error_callback);
        socket_.accept_(server_);
    }*/
    system("pause");
}
