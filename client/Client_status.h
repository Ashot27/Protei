#pragma once

#include <ostream>

enum class status : uint8_t
{
    connected = 0,
    err_socket_init = 1,
    err_socket_connection = 2,
    disconnected = 3
};

inline std::ostream& operator<<(std::ostream& out, const status& _status) {
    const char* s = 0;
#define PROCESS_VAL(p) \
    case (p):          \
        s = #p;        \
        break;
    switch (_status) {
        PROCESS_VAL(status::connected);
        PROCESS_VAL(status::err_socket_init);
        PROCESS_VAL(status::err_socket_connection);
        PROCESS_VAL(status::disconnected);
    }
#undef PROCESS_VAL
    return out << s;
}