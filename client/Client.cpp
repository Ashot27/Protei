#include "Client.h"

Client::Client(const char* destination_ip, const uint16_t destination_port)
    : destination_ip(destination_ip), destination_port(destination_port) {}

Client::~Client() {}

status Client::prepare() {

    if ((c_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cerr << "Failed to create socket: " << strerror(errno) << endl;
        return _status = status::err_socket_init;
    }

    else {
        cout << "The socket is created" << endl;
    }

    struct sockaddr_in destination_addr;
    destination_addr.sin_family = AF_INET;  // IPv4
    destination_addr.sin_port = htons(destination_port);
    destination_addr.sin_addr.s_addr = inet_addr(destination_ip);

    if (connect(c_socket, (struct sockaddr*)&destination_addr,
                sizeof(destination_addr)) < 0) {

        cerr << "Failed to connect to the server " << destination_ip << ":"
             << +destination_port << " :" << strerror(errno) << endl;
        close(c_socket);
        return _status = status::err_socket_connection;
    } else {
        cout << "The socket is connected" << endl;
    }

    return _status = status::connected;
}

void Client::run() {
    connect_hndl();
};

void Client::connect_hndl() {
    string message = "hello";
    send_request_to_server(message);

    auto new_message = resv_response_from_server();
    cout << new_message << endl;
};

void Client::send_request_to_server(string message) {
    size_t length = message.length() + 1;
    char c_message[length];
    for (size_t i = 0; i < length; i++) {
        c_message[i] = message[i];
    }
    send(c_socket, c_message, length, 0);
    cout << "Send " << length << " bytes to server." << endl;
};

string Client::resv_response_from_server() {
    char buffer[BUFFER_SIZE] = {0};
    int resv_bytes_count = 0;
    if ((resv_bytes_count = recv(c_socket, buffer, BUFFER_SIZE, 0)) < 0) {
        return "";
    }
    cout << "Reseive " << resv_bytes_count << " bytes from server." << endl;
    return string(buffer, resv_bytes_count);
};

void Client::stop() {
    _status = status::disconnected;
    if (close(c_socket) < 0) {
        cout << "Failed to close socket: " << strerror(errno) << endl;
    }
};
