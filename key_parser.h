#pragma once

#include <arpa/inet.h>  //inet_aton
#include <getopt.h>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8080

class ParserException : public std::exception {};

struct IPv4_addr {
   private:
    char* ip = DEFAULT_IP;
    int port = DEFAULT_PORT;
    bool is_tcp = true;

   public:
    bool set_ip(char* ip);
    bool set_port(char* str_port);
    bool set_is_tcp(bool is_tcp);
    bool isIP(char* ip);
    char* get_ip();
    int get_port();
    bool get_is_tcp();
};

IPv4_addr parser(int argc, char* argv[], IPv4_addr* addr);
