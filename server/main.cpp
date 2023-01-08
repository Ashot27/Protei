#include "main.h"

int main(int argc, char* argv[]) {

    IPv4_addr addr = IPv4_addr();
    try {
        parser(argc, argv, &addr);
    } catch (ParserException) {
        return 1;
    }
    cout << "Deploing the server on:    " << addr.get_ip() << ":"
         << addr.get_port() << endl;

    Server server = Server::get_instance(addr.get_ip(), addr.get_port());
    Server_status _status;
    if ((_status = server.prepare()) == Server_status::up) {
        cout << "The server is running" << endl;
        cout << "The server has :" << _status << endl;
        server.run();
    } else {
        cerr << "The server has an issue: " << _status << endl;
        return 1;
    }

    return 0;
}