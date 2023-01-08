#include "main.h"

int main(int argc, char* argv[]) {

    IPv4_addr addr = IPv4_addr();
    try {
        parser(argc, argv, &addr);
    } catch (ParserException) {
        return 1;
    }
    cout << "Destination server is:    " << addr.get_ip() << ":"
         << addr.get_port() << endl;

    Client client(addr.get_ip(), addr.get_port(), addr.get_is_tcp());
    Client_status _status = client.prepare();
    if (client.is_status_ok()) {
        cout << "The client is running" << endl;
        client.run();
    } else {
        cerr << "The client has an issue: " << _status << endl;
        return 1;
    }
    return 0;
}