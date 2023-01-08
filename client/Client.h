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

    Client_status prepare();
    void run();
    void data_transmission(const string& message, string& result);
    bool is_status_ok();
    void tcp_connect_hndl(const string& message, string& result);
    void send_request_to_tcp_server(const string& message);
    void resv_response_from_tcp_server(string& result);
    void udp_connect_hndl(const string& message, string& result);
    void send_request_to_udp_server(const string& message);
    void resv_response_from_udp_server(string& result);

   private:
    void stop();

   private:
    struct sockaddr_in destination_addr;
    bool is_tcp;
    int c_socket;
    Client_status _status;
};