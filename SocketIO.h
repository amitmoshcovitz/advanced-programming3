#ifndef _SOCKET_IO
#define _SOCKET_IO
#include "DefaultIO.h"
#include <string>

class SocketIO : public DefaultIO {
    private:
    int socket;
    public:
    SocketIO();
    void write(std::string) const;
    std::string read() const;
    void setSocket(int sock);
};
#endif