#include "main.h"

int main(int argc, char* argv[]) {

    Server server = Server::get_instance("127.0.0.1", 8080);
    Server_status _status;
    if ((_status = server.prepare()) == Server_status::up) {
        cout << "The server is running" << endl;
        cout << "The server has :" << _status << endl;
        server.run();
    } else {
        cerr << "The server has an issue: " << _status << endl;
        server.stop();
        return 1;
    }

    server.stop();
    return 0;
}