#include "key_parser.h"

bool IPv4_addr::set_ip(char* ip) {
    if (isIP(ip)) {
        this->ip = ip;
        return true;
    }
    return false;
}

bool IPv4_addr::set_port(char* str_port) {
    return this->port = atoi(str_port);
}

bool IPv4_addr::set_is_tcp(bool is_tcp) {
    return this->is_tcp = is_tcp;
};

bool IPv4_addr::isIP(char* ip) {
    return (inet_aton(
        ip,
        nullptr));  //not care about the result. just want to check is it an IP or not
}

char* IPv4_addr::get_ip() {
    return this->ip;
}

int IPv4_addr::get_port() {
    return this->port;
}

bool IPv4_addr::get_is_tcp() {
    return this->is_tcp;
}

IPv4_addr parser(int argc, char* argv[], IPv4_addr* addr) {
    const char* short_options =
        "i:p:uh";  // "" after short code - no argument, ":" - required_argument, "::" - optional_argument
    const struct option long_options[] = {
        {"ip", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {"udp", no_argument, NULL, 'u'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}};

    int result;
    int option_index;

    while ((result = getopt_long(argc, argv, short_options, long_options,
                                 &option_index)) != -1) {

        switch (result) {
            case 'h': {
                cout << "Usage: " << argv[0]
                     << " [-ip <ip>] [-port <port>] [-udp]" << std::endl;
                cout << "The -udp key is ignored for the server" << endl;
                throw ParserException();
            };
            case 'i': {
                if (optarg != NULL) {
                    //cout << "Server ip: " << optarg << endl;
                    if (!addr->set_ip(optarg)) {
                        cerr << "Incorrect IP address" << endl;
                        throw ParserException();
                    };
                } else
                    cerr << "Please set IP address for -i/-ip key" << endl;
                break;
            };

            case 'p': {
                if (optarg != NULL) {
                    //cout << "Server port: " << optarg << endl;
                    if (!addr->set_port(optarg)) {
                        cerr << "Incorrect port number" << endl;
                        throw ParserException();
                    };
                } else
                    cerr << "Please set PORT number for -p/-port key" << endl;
                break;
            };
            case 'u': {
                addr->set_is_tcp(false);
                break;
            };
            case '?':
            default: {
                cerr << "Found unknown option. Use -h/-help to know more"
                     << endl;
                throw ParserException();
            };
        };
    };
}