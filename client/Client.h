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
    Client(const char* destination_ip, const uint16_t destination_port);
    ~Client();

    status prepare();
    void run();
    void stop();
    void connect_hndl();
    void send_request_to_server(string message);
    string resv_response_from_server();

   private:
    const char* destination_ip;
    uint16_t destination_port;
    int c_socket;
    status _status;
};