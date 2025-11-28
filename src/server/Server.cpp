#include "Server.hpp"
#include "TintinReporter.hpp"
#include "SignalHandler.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <algorithm>

Server::Server(int port) : _port(port), _serverSocket(-1), _running(false) {}

Server::Server(const Server& other) {
    (void)other;
}

Server& Server::operator=(const Server& other) {
    if (this != &other) {}
    return *this;
}

Server::~Server() {
    stop();
}

bool Server::init() {
    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Creating server.");

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot create socket.");
        return false;
    }

    int opt = 1;

    setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot bind socket.");
        close(_serverSocket);
        return false;
    }

    if (listen(_serverSocket, MAX_CLIENTS) == -1) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot listen on socket.");
        close(_serverSocket);
        return false;
    }

    int flags = fcntl(_serverSocket, F_GETFL, 0);

    fcntl(_serverSocket, F_SETFL, flags | O_NONBLOCK);

    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Server created.");

    return true;
}

void Server::run() {
    _running = true;

    fd_set          readfds;
    struct timeval  tv;

    while (_running && !SignalHandler::shouldQuit()) {
        FD_ZERO(&readfds);
        FD_SET(_serverSocket, &readfds);

        int maxFd = _serverSocket;

        for (size_t i = 0; i < _clients.size(); i++) {
            FD_SET(_clients[i], &readfds);
            if (_clients[i] > maxFd) {
                maxFd = _clients[i];
            }
        }

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int activity = select(maxFd + 1, &readfds, NULL, NULL, &tv);

        if (activity < 0) {
            continue;
        }
        
        if (FD_ISSET(_serverSocket, &readfds)) {
            struct sockaddr_in  clientAddr;
            socklen_t           clientLen = sizeof(clientAddr);
            int                 clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

            if (clientSocket >= 0) {
                if (_clients.size() < MAX_CLIENTS) {
                    _clients.push_back(clientSocket);
                    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: New client connected.");
                } else {
                    TintinReporter::getInstance().log(TintinReporter::WARNING, "Matt_daemon: Max clients reached. Connection refused.");
                    close(clientSocket);
                }
            }
        }
        
        for (size_t i = 0; i < _clients.size(); ) {
            if (FD_ISSET(_clients[i], &readfds)) {
                if (!handleClient(_clients[i])) {
                    removeClient(_clients[i]);
                    _clients.erase(_clients.begin() + i);
                    continue;
                }
            }
            i++;
        }
    }
}

bool Server::handleClient(int clientSocket) {
    char    buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    if (bytesRead <= 0) {
        return false;
    }

    std::string message(buffer, bytesRead);

    message = trim(message);

    if (message == "quit") {
        TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Request quit.");
        _running = false;
        return false;
    }

    if (!message.empty()) {
        TintinReporter::getInstance().log(TintinReporter::LOG, "Matt_daemon: User input: " + message);
    }

    return true;
}

void Server::removeClient(int clientSocket) {
    close(clientSocket);
    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Client disconnected.");
}

std::string Server::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");

    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\n\r");

    return str.substr(first, (last - first + 1));
}

void Server::stop() {
    _running = false;

    for (size_t i = 0; i < _clients.size(); i++) {
        close(_clients[i]);
    }
    _clients.clear();

    if (_serverSocket != -1) {
        close(_serverSocket);
        _serverSocket = -1;
    }
}
