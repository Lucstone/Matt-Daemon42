#pragma once

#include <string>
#include <vector>

class Server {
public:
    Server(int port);
    ~Server();

    bool        init();
    void        run();
    void        stop();

private:
    static const int MAX_CLIENTS = 3;
    static const int BUFFER_SIZE = 1024;

    bool        handleClient(int clientSocket);
    void        removeClient(int clientSocket);
    std::string trim(const std::string& str);

    int                 _port;
    int                 _serverSocket;
    std::vector<int>    _clients;
    bool                _running;
};
