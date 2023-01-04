#pragma once

#include "Server_status.h"

#include <arpa/inet.h>   // inet_addr
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // close
#include <cstring>   //strerror
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::strerror;
using std::string;

#define BUFFER_SIZE 1024

class Server {
   private:
    Server(const char* ip, const uint16_t port);

   public:
    ~Server();
    static Server& get_instance(const char* ip, const uint32_t port);
    status prepare();
    void run();
    void stop();
    void connect_hndl(const int client_socket);
    string resv_request_from_client(const int client_socket);
    void send_response_to_client(const int client_socket, string data);

   private:
    status _status;
    int s_socket_tcp;
    int s_socket_udp;
    int max_fd;
    fd_set fd_read;
    fd_set fd_write;
    sockaddr_in addr;
};