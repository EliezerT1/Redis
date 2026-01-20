#include "include/RedisServer.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>


static RedisServer* globalServer = nullptr;

RedisServer::RedisServer(int port) : port(port), server_socket(-1), running(true) {
    globalServer = this;
}

void RedisServer::shutdown() {
    running = false;
    if(server_socket != -1){
        close(server_socket);
    }
    std::cout << "Server shutdown Complete!\n";
}

void RedisServer::run() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error Creating Server Socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverADDR;
    serverADDR.sin_family = AF_INET;
    serverADDR.sin_port = htons(port);
    serverADDR.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*)&serverADDR, sizeof(serverADDR)) < 0) {
        std::cerr << "Error Binding Server Socket\n";
        return;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Error Listening On Server Socket\n";
        return;
    }

    std::cout << "Redis Server listening on port" << port << "\n";

    
}