#include "Client.h"

Client::Client(const char* destination_ip, const uint16_t destination_port,
               bool is_tcp /*= true*/) {
    destination_addr.sin_family = AF_INET;  // IPv4
    destination_addr.sin_port = htons(destination_port);
    destination_addr.sin_addr.s_addr = inet_addr(destination_ip);
    this->is_tcp = is_tcp;
}

Client::~Client() {
    stop();
}

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
                 << htons(destination_addr.sin_port) << " : " << strerror(errno)
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
    string message;
    cout << "Write \":q!\" to exit or use Ctrl+C" << endl;
    while (is_status_ok()) {
        getline(cin, message);
        if (message == ":q!") {
            break;
        }
        string result;
        data_transmission(message, result);
        cout << "The response from the server:" << endl << result << endl;
    }
};

void Client::data_transmission(const string& message, string& result) {
    // It has been allocated to a separate function for testing
    if (is_tcp) {
        tcp_connect_hndl(message, result);
    } else {
        udp_connect_hndl(message, result);
    }
}

bool Client::is_status_ok() {
    if (is_tcp) {
        return _status == Client_status::connected;
    }
    return _status == Client_status::up;
}

void Client::tcp_connect_hndl(const string& message, string& result) {
    send_request_to_tcp_server(message);
    resv_response_from_tcp_server(result);
};

void Client::send_request_to_tcp_server(const string& message) {
    size_t length = message.length() + 1;  // +1 for null terminator
    send(c_socket, message.c_str(), length, 0);
};

void Client::resv_response_from_tcp_server(string& result) {
    int resv_bytes_count =
        0;  // the number of bytes received from a single message
    char buffer[BUFFER_SIZE + 1] = {0};
    do {
        resv_bytes_count = recv(c_socket, buffer, BUFFER_SIZE, 0);
        if (resv_bytes_count < 0) {
            cout << "The connection with TCP server was closed" << endl;
            _status == Client_status::disconnected;
            break;  // ignore the rest message
        }
        result += buffer;
    } while (resv_bytes_count == BUFFER_SIZE);
};

void Client::udp_connect_hndl(const string& message, string& result) {
    send_request_to_udp_server(message);
    resv_response_from_udp_server(result);
};

void Client::send_request_to_udp_server(const string& message) {
    string tmp = "";
    size_t length = 0;
    //we separete the message by size if it's bigger than reading buffer size
    for (size_t i = 0; i < message.length(); i += BUFFER_SIZE) {
        tmp = message.substr(i, BUFFER_SIZE);
        length = tmp.length() + 1;  // +1 for null terminator
        sendto(c_socket, tmp.c_str(), length, 0,
               (struct sockaddr*)&destination_addr, sizeof(sockaddr_in));
    }
};

void Client::resv_response_from_udp_server(string& result) {
    socklen_t addr_len = sizeof(sockaddr_in);
    int resv_bytes_count =
        0;  // the number of bytes received from a single message
    char buffer[BUFFER_SIZE + 1] = {0};
    do {
        resv_bytes_count =
            recvfrom(c_socket, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr*)&destination_addr, &addr_len);
        if (resv_bytes_count < 0) {
            _status == Client_status::disconnected;
            break;  // ignore the rest message
        }
        result += buffer;
    } while (resv_bytes_count == BUFFER_SIZE);
};

void Client::stop() {
    _status = Client_status::disconnected;
    if (c_socket && close(c_socket) < 0) {
        cout << "Failed to close socket: " << strerror(errno) << endl;
    }
};
