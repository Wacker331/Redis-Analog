#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>

#include "logger.h"
#include "storage.h"

//Server class with storage, logger and main socket
class Server
{
    Logger &Logs;
    Storage& MainStorage;
    int AcceptSock;
    int MaxConnections;
public:
    Server(const int, Storage&,  Logger&, const int);
    ~Server();
    void ServerMainThread();
};

//Client class with storage, logs, socket and clients' address
class Client
{
    int Socket;
    struct sockaddr_in Address;
    Storage& MainStorage;
    Logger& Logs;
    friend class Client_Test;
protected:
    int RequestHandler(char*);
    std::vector<std::string> ParseInput(char*);
public:
    Client(const int, const struct sockaddr_in, Storage&, Logger&);
    ~Client();
    void ClientMainThread();

};

#endif