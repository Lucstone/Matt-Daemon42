#pragma once

#include <vector>

class Server {
private:
    int                 _listenFd;
    int                 _port;
    std::vector<int>    _clients;
    
    void                setupSocket_();
    void                handleClient_(int clientFd);
    
public:
    Server();
    explicit Server(int port);
    Server(const Server& other);
    Server& operator=(const Server& other);
    ~Server();

    void                run(); 
};
