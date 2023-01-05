#include "Server.h"

Server::Server(const char* ip, const uint16_t port) {
    addr.sin_family = AF_INET;  // IPv4
    addr.sin_port = htons(port);
    // htos convert 16-bit quantities between network byte order and host byte order.
    addr.sin_addr.s_addr = inet_addr(ip);
}

Server::~Server() {
    if (_status == Server_status::up)
        stop();
}

Server& Server::get_instance(const char* ip, const uint32_t port) {
    static Server instance(ip, port);
    return instance;
}

Server_status Server::prepare() {
    // TCP
    if ((s_socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cerr << "Failed to create TCP socket: " << strerror(errno) << endl;
        return _status = Server_status::err_socket_init;
    } else {
        cout << "The TCP socket is created" << endl;
    }

    int flag = true;
    if ((setsockopt(s_socket_tcp, SOL_SOCKET, SO_REUSEADDR, &flag,
                    sizeof(flag)) == -1) ||
        (setsockopt(s_socket_tcp, SOL_SOCKET, SO_REUSEPORT, &flag,
                    sizeof(flag)) == -1)) {

        cerr << "Failed to set options to TCP socket: " << strerror(errno)
             << endl;
        return _status = Server_status::err_socket_setopt;
    } else {
        cout << "The TCP socket options were set" << endl;
    }

    if (bind(s_socket_tcp, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Failed to bind TCP socket: " << strerror(errno) << endl;
        return _status = Server_status::err_socket_bind;
    } else {
        cout << "The TCP socket is binded" << endl;
    }

    if (listen(s_socket_tcp, SOMAXCONN) < 0) {  // ~ 128 (based on the system)
        cerr << "Failed to listen socket: " << strerror(errno) << endl;
        return _status = Server_status::err_socket_listening;
    } else {
        cout << "The socket is listening" << endl;
    }

    // UDP
    if ((s_socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        cerr << "Failed to create UDP socket: " << strerror(errno) << endl;
        return _status = Server_status::err_socket_init;
    } else {
        cout << "The UDP socket is created" << endl;
    }

    if ((setsockopt(s_socket_udp, SOL_SOCKET, SO_REUSEADDR, &flag,
                    sizeof(flag)) == -1) ||
        (setsockopt(s_socket_udp, SOL_SOCKET, SO_REUSEPORT, &flag,
                    sizeof(flag)) == -1)) {

        cerr << "Failed to set options to UDP socket: " << strerror(errno)
             << endl;
        return _status = Server_status::err_socket_setopt;
    } else {
        cout << "The UDP socket options were set" << endl;
    }

    if (bind(s_socket_udp, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Failed to bind UDP socket: " << strerror(errno) << endl;
        return _status = Server_status::err_socket_bind;
    } else {
        cout << "The UDP socket is binded" << endl;
    }

    FD_ZERO(&fd_read);  // clear the descriptor
    max_fd = (s_socket_tcp > s_socket_udp ? s_socket_tcp : s_socket_udp) + 1;

    return _status = Server_status::up;
}

void Server::run() {

    while (_status == Server_status::up) {

        // set listenfd and udpfd in readset
        FD_SET(s_socket_tcp, &fd_read);
        FD_SET(s_socket_udp, &fd_read);

        // select the ready descriptor
        int ready = select(max_fd, &fd_read, NULL, NULL, NULL);

        if (FD_ISSET(s_socket_tcp, &fd_read)) {  // if tcp socket is readable
            tcp_connect_hndl();
        }
        if (FD_ISSET(s_socket_udp, &fd_read)) {  // if udp socket is readable
            udp_connect_hndl();
        }
    }
};

void Server::tcp_connect_hndl() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(sockaddr_in);
    int client_socket =
        accept(s_socket_tcp, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket >= 0 && _status == Server_status::up) {
        cout << "New connection!" << endl;
        auto data = resv_request_from_tcp_client(client_socket);
        send_response_to_tcp_client(client_socket, data);
    }
}

string Server::resv_request_from_tcp_client(const int client_socket) {
    int resv_bytes_count = 0;
    char buffer[BUFFER_SIZE] = {0};
    if ((resv_bytes_count = recv(client_socket, buffer, BUFFER_SIZE, 0)) < 0) {
        return "";
    }
    cout << "Reseive " << resv_bytes_count << " bytes from client." << endl;
    return string(buffer, resv_bytes_count);
}

void Server::send_response_to_tcp_client(const int client_socket, string data) {
    size_t length = data.length();
    char* c_message = new char[length + 1];
    strcpy(c_message, data.c_str());

    send(client_socket, c_message, length, 0);
    cout << "Send " << length << " bytes to client." << endl;
    delete[] c_message;
}

void Server::udp_connect_hndl() {
    struct sockaddr_in client_addr;
    auto data = resv_request_from_udp_client(client_addr);
    send_response_to_udp_client(client_addr, data);
}

string Server::resv_request_from_udp_client(
    const struct sockaddr_in& client_addr) {
    socklen_t addr_len = sizeof(sockaddr_in);
    int resv_bytes_count = 0;
    char buffer[BUFFER_SIZE] = {0};
    if ((resv_bytes_count =
             recvfrom(s_socket_udp, buffer, BUFFER_SIZE, 0,
                      (struct sockaddr*)&client_addr, &addr_len)) < 0) {
        return "";
    }
    cout << "Reseive " << resv_bytes_count << " bytes from client." << endl;
    return string(buffer, resv_bytes_count);
}

void Server::send_response_to_udp_client(const struct sockaddr_in& client_addr,
                                         string data) {
    size_t length = data.length();
    char* c_message = new char[length + 1];
    strcpy(c_message, data.c_str());

    sendto(s_socket_udp, c_message, length, 0, (struct sockaddr*)&client_addr,
           sizeof(sockaddr_in));
    cout << "Send " << length << " bytes to client." << endl;
    delete[] c_message;
}

void Server::stop() {
    _status = Server_status::down;
    if (close(s_socket_tcp) < 0) {
        cout << "Failed to close TCP socket: " << strerror(errno) << endl;
    }
    if (close(s_socket_udp) < 0) {
        cout << "Failed to close UDP socket: " << strerror(errno) << endl;
    }
}