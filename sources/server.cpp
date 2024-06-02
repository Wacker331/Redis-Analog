#include "server.h"

//Global variable to track connection's number
int CurrentConnections = 0;
std::mutex MutexCurrentConnections;

//Function for std::thread to init Client thread
void InitClient(Client *NewClient)
{
    NewClient -> ClientMainThread();
    MutexCurrentConnections.lock();
    CurrentConnections--;
    MutexCurrentConnections.unlock();
}

//Init and bind server to listening socket with port
Server::Server(const int Port, Storage& InputStorage, Logger& InputLogger, const int MaxClients) : MainStorage(InputStorage), Logs(InputLogger)
{
    MaxConnections = MaxClients;
    CurrentConnections = 0;
    struct sockaddr_in Address;
    int AddrLen = sizeof(Address);

    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(Port);

    if ((AcceptSock = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        std::cout << "Can't create socket" << std::endl;
        return;
    }

    if (bind(AcceptSock, (struct sockaddr *)&Address, AddrLen) < 0) 
    {
        std::cout << "Can't bind socket on " << Port << " port" << std::endl;
        return;
    }
    std::cout << "Server started on " << Port << " port" << std::endl;
    Logs << "Server started on " + std::to_string(Port) + " port";
    //Starts server
    ServerMainThread();
}

//Close listening socket
Server::~Server()
{
    close(AcceptSock);
}

//Main thread of server. Listening, accepting connections and creating client's threads.
void Server::ServerMainThread()
{
    int NewConnection;
    struct sockaddr_in Address;
    int AddrLen = sizeof(Address);

    while (1)
    {
        if (listen(AcceptSock, MaxConnections + 1) < 0)
        {
            Logs << LogMessage(CRITICAL, "Listen error");
            return;
        }
        else 
        {
            MutexCurrentConnections.lock();
            if (CurrentConnections < MaxConnections)
            {
                MutexCurrentConnections.unlock();
                if ((NewConnection = accept(AcceptSock, (struct sockaddr *)&Address, (socklen_t*)&AddrLen)) < 0) 
                {
                    Logs << LogMessage(ERROR, "Socket accept error");
                }
                else
                {
                    char TextAddress[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &Address.sin_addr, TextAddress, INET_ADDRSTRLEN);
                    Logs << LogMessage(INFO, std::string("New connection from ") + TextAddress + ":" + std::to_string(ntohs(Address.sin_port)));

                    MutexCurrentConnections.lock();
                    CurrentConnections++;
                    if (CurrentConnections == MaxConnections)
                        Logs << LogMessage(WARNING, "Connections number reached it's maximum");
                    MutexCurrentConnections.unlock();

                    Client *NewClient = new Client(NewConnection, Address, MainStorage, Logs);
                    std::thread *ClientThread = new std::thread(InitClient, NewClient);
                }
            }
            else
            {
                MutexCurrentConnections.unlock();
            }
        }
    }
}

//Inits Client class with exact socket
Client::Client(const int InputSocket, const struct sockaddr_in InputAddress, Storage& InputStorage,
                Logger& InputLogger) : MainStorage(InputStorage), Logs(InputLogger)
{
    Socket = InputSocket;
    Address = InputAddress;
}

//Closes client socket
Client::~Client()
{
    close(Socket);
}

//Parsing input string from client to arguments by ' '
std::vector<std::string> Client::ParseInput(char* buffer)
{
    std::vector<std::string> RetVector;
    char* WordStart = buffer;
    while (*buffer != '\0')
    {
        if (*buffer == '\r')
            *buffer = '\0';
        if (*buffer == ' ' || *buffer == '\x0A')
        {
            *buffer = '\0';
            if (buffer - WordStart > 1)
            {
                RetVector.push_back(std::string(WordStart));
            }
            WordStart = buffer + 1;
        }
        buffer++;
    }
    return RetVector;
}

//Handling requests from clientside and creates answer
int Client::RequestHandler(char* buffer)
{
    Elem RetElem;
    std::vector<std::string> Arguments = ParseInput(buffer);
    memset(buffer, '\0', 512);
    std::string AnswerStr = "OK";
    if (Arguments.size() == 0)
        return -1;
    if (Arguments[0] == std::string("PUT"))
    {
        if (Arguments.size() >= 3)
        {
            RetElem = MainStorage.Put(Elem(Arguments[1], Arguments[2]));
        }
        else
        {
            Logs << LogMessage(WARNING, "Too few arguments in PUT command");
            return -1;
        }
    }
    else if (Arguments[0] == "GET")
    {
        if (Arguments.size() >= 2)
        {
            RetElem = MainStorage[Arguments[1]];
            if (RetElem.Key == "")
                AnswerStr = "NE";
        }
        else
        {
            Logs << LogMessage(WARNING, "Too few arguments in GET command");
            return -1;
        }
    }
    else if (Arguments[0] == "DEL")
    {
        if (Arguments.size() >= 2)
        {
            RetElem = MainStorage.Del(Arguments[1]);
            if (RetElem.Key == "")
                AnswerStr = "NE";
        }
        else
        {
            Logs << LogMessage(WARNING, "Too few arguments in DEL command");
            return -1;
        }
    }
    else if (Arguments[0] == "COUNT")
    {
        RetElem = MainStorage.GetCount();
        if (RetElem.Value == "")
        {
            Logs << LogMessage(WARNING, "Can't get \"CountElem\" from \"Storage\"");
            return -1;
        }
    }
    else if (Arguments[0] == "DUMP")
    {
        if (Arguments.size() >= 2)
        {
            MainStorage.Dump(Arguments[1]);
            Logs << "Storage was dumped";
        }
    }
    else 
    {
        Logs << LogMessage(WARNING, std::string("Wrong command receieved") + buffer);
        AnswerStr = "";
        RetElem.Value = "";
    }
    strcpy(buffer, (AnswerStr + " " + RetElem.Value + "\n").c_str());
    return 0;
}

//Main thread for client. Just receiving buffer and provide it to handler
void Client::ClientMainThread()
{
    char buffer[512];
    int RecvBytes = 1;
    while(RecvBytes > 0)
    {
        RecvBytes = recv(Socket, buffer, 512, 0);
        if (RecvBytes > 0)
        {
            RequestHandler(buffer);
            send(Socket, buffer, 512, 0);
        }
    }
    char TextAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &Address.sin_addr, TextAddress, INET_ADDRSTRLEN);
    Logs << LogMessage(INFO, std::string("Client ") + TextAddress + ":" \
                         + std::to_string(ntohs(Address.sin_port)) + " disconnected");
}