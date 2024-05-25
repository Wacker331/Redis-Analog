#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <cstring>

#include "logger.h"
#include "storage.h"

#define DEFAULT_CONNECTION_MAX 10

class Server
{
    Logger &Logs;
    Storage& MainStorage;
    int AcceptSock;
    int MaxConnections, CurrentConnections;
public:
    Server(int, Storage&, Logger&);
    ~Server();
    void ServerMainThread();
};

class Client
{
    int Socket;
    struct sockaddr_in Address;
    Storage& MainStorage;
    Logger& Logs;
public:
    int RequestHandler(char*);
    std::vector<std::string> ParseInput(char*);
    Client(int, struct sockaddr_in, Storage&, Logger&);
    ~Client();
    void ClientMainThread();
};

#endif