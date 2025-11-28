#include "Daemon.hpp"
#include "TintinReporter.hpp"
#include "SignalHandler.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

Daemon::Daemon() : _server(NULL), _lockFile(NULL) {}

Daemon::~Daemon() {
    delete _server;
    delete _lockFile;
}

bool Daemon::checkRoot() {
    if (getuid() != 0) {
        std::cerr << "Error: This daemon must be run as root." << std::endl;
        return false;
    }
    return true;
}

bool Daemon::init() {
    // Vérifier les droits root
    if (!checkRoot()) {
        return false;
    }
    
    // Initialiser le logger
    if (!TintinReporter::getInstance().init()) {
        return false;
    }
    
    TintinReporter::getInstance().log(TintinReporter::INFO, 
        "Matt_daemon: Started.");
    
    // Créer le fichier de lock
    _lockFile = new LockFile();
    if (!_lockFile->create()) {
        TintinReporter::getInstance().log(TintinReporter::INFO, 
            "Matt_daemon: Quitting.");
        return false;
    }
    
    // Créer le serveur
    _server = new Server(4242);
    if (!_server->init()) {
        return false;
    }
    
    // Configurer les gestionnaires de signaux
    SignalHandler::setup();
    
    // Daemoniser
    if (!daemonize()) {
        return false;
    }
    
    return true;
}

bool Daemon::daemonize() {
    TintinReporter::getInstance().log(TintinReporter::INFO, 
        "Matt_daemon: Entering Daemon mode.");
    
    // Fork 1
    pid_t pid = fork();
    if (pid < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, 
            "Matt_daemon: Fork failed.");
        return false;
    }
    
    if (pid > 0) {
        // Processus parent: quitter
        exit(0);
    }
    
    // Créer une nouvelle session
    if (setsid() < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, 
            "Matt_daemon: setsid failed.");
        return false;
    }
    
    // Fork 2
    pid = fork();
    if (pid < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, 
            "Matt_daemon: Second fork failed.");
        return false;
    }
    
    if (pid > 0) {
        // Processus parent: quitter
        exit(0);
    }
    
    // Changer le répertoire de travail
    chdir("/");
    
    // Définir le umask
    umask(0);
    
    // Fermer les descripteurs de fichier standard
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // Rediriger vers /dev/null
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) {
        close(fd);
    }
    
    // Logger le PID
    std::ostringstream oss;
    oss << "Matt_daemon: started. PID: " << getpid() << ".";
    TintinReporter::getInstance().log(TintinReporter::INFO, oss.str());
    
    return true;
}

void Daemon::run() {
    if (_server) {
        _server->run();
    }
    
    TintinReporter::getInstance().log(TintinReporter::INFO, 
        "Matt_daemon: Quitting.");
}
