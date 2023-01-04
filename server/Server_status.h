#pragma once

#include <ostream>

enum class status : uint8_t
{
    up = 0,
    err_socket_init = 1,
    err_socket_setopt = 2,
    err_socket_bind = 3,
    err_scoket_keep_alive = 4,
    err_socket_listening = 5,
    down = 6
};

inline std::ostream& operator<<(std::ostream& out, const status& _status) {
    const char* s = 0;
#define PROCESS_VAL(p) \
    case (p):          \
        s = #p;        \
        break;
    switch (_status) {
        PROCESS_VAL(status::up);
        PROCESS_VAL(status::err_socket_init);
        PROCESS_VAL(status::err_socket_setopt);
        PROCESS_VAL(status::err_socket_bind);
        PROCESS_VAL(status::err_scoket_keep_alive);
        PROCESS_VAL(status::err_socket_listening);
        PROCESS_VAL(status::down);
    }
#undef PROCESS_VAL
    return out << s;
}