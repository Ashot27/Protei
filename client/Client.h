#pragma once

#include <arpa/inet.h>   //inet_addr
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // close
#include <cstring>   //strerror
#include <iostream>

#include "Client_status.h"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::strerror;
using std::string;

#define BUFFER_SIZE 1024

class Client {
   public:
    Client(const char* destination_ip, const uint16_t destination_port,
           bool is_tcp = true);
    ~Client();

    status prepare();
    void run();
    bool is_status_ok();
    void stop();
    void tcp_connect_hndl();
    void send_request_to_tcp_server(string message);
    string resv_response_from_tcp_server();
    void udp_connect_hndl();
    void send_request_to_udp_server(string message);
    string resv_response_from_udp_server();

   private:
    struct sockaddr_in destination_addr;
    bool is_tcp;
    int c_socket;
    status _status;
};