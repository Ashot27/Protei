#include "main.h"

int main(int argc, char* argv[]) {

    Client client("127.0.0.1", 8080, true);

    status _status = client.prepare();
    if (client.is_status_ok()) {
        cout << "The client is running" << endl;
        client.run();
    } else {
        cerr << "The client has an issue: " << _status << endl;
        client.stop();
        return 1;
    }

    client.stop();
    return 0;
}