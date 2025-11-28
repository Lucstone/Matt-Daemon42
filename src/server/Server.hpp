#pragma once

#include <string>
#include <vector>

class Server {
private:
    static const int MAX_CLIENTS = 3;
    static const int BUFFER_SIZE = 1024;

    int                 _port;
    int                 _serverSocket;
    std::vector<int>    _clients;
    bool                _running;

    bool        handleClient(int clientSocket);
    void        removeClient(int clientSocket);
    std::string trim(const std::string& str);

public:
    Server(int port);
    ~Server();

    bool        init();
    void        run();
    void        stop();
};
