#pragma once

#include <ostream>

enum class Client_status : uint8_t
{
    up = 0,
    err_socket_init = 1,
    err_socket_connection = 2,
    connected = 3,
    disconnected = 4
};

inline std::ostream& operator<<(std::ostream& out,
                                const Client_status& _status) {
    const char* s = 0;
#define PROCESS_VAL(p) \
    case (p):          \
        s = #p;        \
        break;
    switch (_status) {
        PROCESS_VAL(Client_status::up);
        PROCESS_VAL(Client_status::err_socket_init);
        PROCESS_VAL(Client_status::err_socket_connection);
        PROCESS_VAL(Client_status::connected);
        PROCESS_VAL(Client_status::disconnected);
    }
#undef PROCESS_VAL
    return out << s;
}