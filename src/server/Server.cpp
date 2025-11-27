#include "server/Server.hpp"
#include "tintin/TintinReporter.hpp"
#include "signals/SignalHandler.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <fcntl.h>

Server::Server()
: _listenFd(-1), _port(4242), _clients()
{
    setupSocket_();
}

Server::Server(int port)
: _listenFd(-1), _port(port), _clients()
{
    setupSocket_();
}

Server::Server(const Server& other): _listenFd(-1), _port(other._port), _clients()
{
    setupSocket_();
}

Server& Server::operator=(const Server& other) {
    if (this != &other) {
        if (_listenFd >= 0)
            ::close(_listenFd);
        for (size_t i = 0; i < _clients.size(); ++i)
            ::close(_clients[i]);
        _clients.clear();

        _port      = other._port;
        _listenFd  = -1;
        setupSocket_();
    }
    return *this;
}

Server::~Server() {
    if (_listenFd >= 0)
        ::close(_listenFd);
    for (size_t i = 0; i < _clients.size(); ++i)
        ::close(_clients[i]);
}
