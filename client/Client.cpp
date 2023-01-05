#include "Client.h"

Client::Client(const char* destination_ip, const uint16_t destination_port,
               bool is_tcp /*= true*/) {
    destination_addr.sin_family = AF_INET;  // IPv4
    destination_addr.sin_port = htons(destination_port);
    destination_addr.sin_addr.s_addr = inet_addr(destination_ip);
    this->is_tcp = is_tcp;
}

Client::~Client() {}

Client_status Client::prepare() {
    if (is_tcp) {
        if ((c_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            cerr << "Failed to create TCP socket: " << strerror(errno) << endl;
            return _status = Client_status::err_socket_init;
        } else {
            cout << "The TCP socket is created" << endl;
        }
        if (connect(c_socket, (struct sockaddr*)&destination_addr,
                    sizeof(destination_addr)) < 0) {
            cerr << "Failed to connect to the server "
                 << inet_ntoa(destination_addr.sin_addr) << ":"
                 << htons(destination_addr.sin_port) << " :" << strerror(errno)
                 << endl;
            close(c_socket);
            return _status = Client_status::err_socket_connection;
        } else {
            cout << "The socket is connected" << endl;
        }
        return _status = Client_status::connected;
    } else {
        if ((c_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            cerr << "Failed to create UDP socket: " << strerror(errno) << endl;
            return _status = Client_status::err_socket_init;
        } else {
            cout << "The UDP socket is created" << endl;
        }
        return _status = Client_status::up;
    }
}

void Client::run() {
    if (is_tcp) {
        tcp_connect_hndl();
    } else {
        udp_connect_hndl();
    }
};

bool Client::is_status_ok() {
    if (is_tcp) {
        return _status == Client_status::connected;
    }
    return _status == Client_status::up;
}

void Client::tcp_connect_hndl() {
    string message = "hello";
    send_request_to_tcp_server(message);

    auto new_message = resv_response_from_tcp_server();
    cout << new_message << endl;
};

void Client::send_request_to_tcp_server(string message) {
    size_t length = message.length() + 1;
    char c_message[length];
    for (size_t i = 0; i < length; i++) {
        c_message[i] = message[i];
    }
    send(c_socket, c_message, length, 0);
    cout << "Send " << length << " bytes to server." << endl;
};

string Client::resv_response_from_tcp_server() {
    char buffer[BUFFER_SIZE] = {0};
    int resv_bytes_count = 0;
    if ((resv_bytes_count = recv(c_socket, buffer, BUFFER_SIZE, 0)) < 0) {
        return "";
    }
    cout << "Reseive " << resv_bytes_count << " bytes from server." << endl;
    return string(buffer, resv_bytes_count);
};

void Client::udp_connect_hndl() {
    string message = "hello";
    send_request_to_udp_server(message);
    auto new_message = resv_response_from_udp_server();
    cout << new_message << endl;
};

void Client::send_request_to_udp_server(string message) {
    size_t length = message.length() + 1;
    char c_message[length];
    for (size_t i = 0; i < length; i++) {
        c_message[i] = message[i];
    }
    sendto(c_socket, c_message, length, 0, (struct sockaddr*)&destination_addr,
           sizeof(sockaddr_in));

    cout << "Send " << length << " bytes to server." << endl;
};

string Client::resv_response_from_udp_server() {
    socklen_t addr_len = sizeof(sockaddr_in);
    char buffer[BUFFER_SIZE] = {0};
    int resv_bytes_count = 0;
    if ((resv_bytes_count =
             recvfrom(c_socket, buffer, BUFFER_SIZE, 0,
                      (struct sockaddr*)&destination_addr, &addr_len)) < 0) {
        return "";
    }
    cout << "Reseive " << resv_bytes_count << " bytes from server." << endl;
    return string(buffer, resv_bytes_count);
};

void Client::stop() {
    _status = Client_status::disconnected;
    if (close(c_socket) < 0) {
        cout << "Failed to close socket: " << strerror(errno) << endl;
    }
};
