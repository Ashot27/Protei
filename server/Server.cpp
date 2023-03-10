#include "Server.h"

Server::Server(const char* ip, const uint16_t port) {
    addr.sin_family = AF_INET;  // IPv4
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
}

Server::~Server() {
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

    return _status = Server_status::up;
}

void Server::run() {

    while (_status == Server_status::up) {

        // set s_socket_tcp and s_socket_udp in readset
        FD_SET(s_socket_tcp, &fd_read);
        FD_SET(s_socket_udp, &fd_read);

        max_fd = (s_socket_tcp > s_socket_udp ? s_socket_tcp : s_socket_udp);
        for (const auto& client : tcp_connections) {
            if (client.socket > 0)
                FD_SET(client.socket, &fd_read);

            if (client.socket > max_fd)
                max_fd = client.socket;
        }

        // select the ready descriptor
        int ready = select(max_fd + 1, &fd_read, NULL, NULL, NULL);
        if (ready < 0) {
            cerr << "Failed to select socket: " << strerror(errno) << endl;
            FD_ZERO(&fd_read);  // clear the descriptor
            continue;
        }

        if (FD_ISSET(s_socket_udp, &fd_read)) {  // if udp socket is readable
            udp_connect_hndl();
        }

        if (FD_ISSET(s_socket_tcp,
                     &fd_read)) {  // if tcp socket gets a new connection
            tcp_new_connect_hndl();
        }
        for (size_t i = 0, n = tcp_connections.size(); i < n;) {
            if (FD_ISSET(tcp_connections[i].socket,
                         &fd_read)) {  // if tcp socket is readable
                if (!tcp_connect_hndl(tcp_connections[i])) {
                    //if the connection was closed;
                    tcp_connections.erase(tcp_connections.begin() + i);
                    n--;
                    continue;
                }
            };
            i++;
        }
    }
};

void Server::tcp_new_connect_hndl() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(sockaddr_in);
    int client_socket =
        accept(s_socket_tcp, (struct sockaddr*)&client_addr, &addr_len);
    cout << "New connection! " << inet_ntoa(client_addr.sin_addr) << ":"
         << htons(client_addr.sin_port) << endl;

    tcp_connections.push_back({client_socket, client_addr});
}

bool Server::tcp_connect_hndl(const client& client) {
    //return false if the connection was closed;
    if (client.socket >= 0 && _status == Server_status::up) {
        string result;
        if (resv_request_from_tcp_client(client.socket, result) > 0) {
            cout << "The message from TCP client "
                 << inet_ntoa(client.addr.sin_addr) << ":"
                 << htons(client.addr.sin_port) << " :  " << result << endl;
            string reply_message = make_correct_response(result);
            send_response_to_tcp_client(client.socket, reply_message);
            return true;

        } else {
            cout << "The connection with TCP client "
                 << inet_ntoa(client.addr.sin_addr) << ":"
                 << htons(client.addr.sin_port) << " was closed" << endl;

            close(client.socket);
            return false;
        }
    }
    return false;
}

int Server::resv_request_from_tcp_client(const int client_socket,
                                         string& result) {
    int resv_bytes_count =
        0;  // the number of bytes received from a single message
    int total_bytes_count = 0;
    char buffer[BUFFER_SIZE + 1] = {0};
    do {
        resv_bytes_count = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (resv_bytes_count < 0) {
            return 0;  // ignore the whole message
        }
        total_bytes_count += resv_bytes_count;
        result += buffer;
    } while (resv_bytes_count == BUFFER_SIZE);
    return total_bytes_count;
}

void Server::send_response_to_tcp_client(const int client_socket,
                                         const string& message) {
    size_t length = message.length() + 1;  // +1 for null terminator
    send(client_socket, message.c_str(), length + 1, 0);
}

void Server::udp_connect_hndl() {
    struct sockaddr_in client_addr;
    string result;
    if (resv_request_from_udp_client(client_addr, result) > 0) {
        cout << "New message from UDP client "
             << inet_ntoa(client_addr.sin_addr) << ":"
             << htons(client_addr.sin_port) << " :  " << result << endl;
        string reply_message = make_correct_response(result);
        send_response_to_udp_client(client_addr, reply_message);
    }
}

int Server::resv_request_from_udp_client(const struct sockaddr_in& client_addr,
                                         string& result) {
    socklen_t addr_len = sizeof(sockaddr_in);
    int resv_bytes_count =
        0;  // the number of bytes received from a single message
    int total_bytes_count = 0;
    char buffer[BUFFER_SIZE + 1] = {0};

    do {
        resv_bytes_count = recvfrom(s_socket_udp, buffer, BUFFER_SIZE, 0,
                                    (struct sockaddr*)&client_addr, &addr_len);
        if (resv_bytes_count < 0) {
            return 0;
        }
        total_bytes_count += resv_bytes_count;
        result += buffer;
    } while (resv_bytes_count == BUFFER_SIZE);
    return total_bytes_count;
}

void Server::send_response_to_udp_client(const struct sockaddr_in& client_addr,
                                         string message) {
    string tmp = "";
    size_t length = 0;
    //we separete the message by size if it's bigger than reading buffer size
    for (size_t i = 0; i < message.length(); i += BUFFER_SIZE) {
        tmp = message.substr(i, BUFFER_SIZE);
        length = tmp.length() + 1;  // +1 for null terminator
        sendto(s_socket_udp, tmp.c_str(), length, 0,
               (struct sockaddr*)&client_addr, sizeof(sockaddr_in));
    }
}

void Server::stop() {
    _status = Server_status::down;
    for (const auto& client : tcp_connections) {
        if (client.socket && close(client.socket) < 0) {
            cout << "Failed to close TCP client socket: " << strerror(errno)
                 << endl;
        }
    }
    if (s_socket_tcp && close(s_socket_tcp) < 0) {
        cout << "Failed to close TCP socket: " << strerror(errno) << endl;
    }
    if (s_socket_udp && close(s_socket_udp) < 0) {
        cout << "Failed to close UDP socket: " << strerror(errno) << endl;
    }
}