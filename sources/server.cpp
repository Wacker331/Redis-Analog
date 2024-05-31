#include "server.h"

void InitClient(Client *NewClient)
{
    NewClient -> ClientMainThread();
}

Server::Server(int Port, Storage& InputStorage, Logger& InputLogger) : MainStorage(InputStorage), Logs(InputLogger)
{
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
        std::cout << "Can't bind socket" << std::endl;
        return;
    }
    ServerMainThread();
}

Server::~Server()
{
    close(AcceptSock);
}

void Server::ServerMainThread()
{
    int NewConnection;
    struct sockaddr_in Address;
    int AddrLen = sizeof(Address);

    while (1)
    {
        if (listen(AcceptSock, 3) < 0)
        {
            Logs << LogMessage(DEBUG, "Listen error");
            return;
        }
        if ((NewConnection = accept(AcceptSock, (struct sockaddr *)&Address, (socklen_t*)&AddrLen)) < 0) 
        {
            Logs << LogMessage(DEBUG, "Socket accept error");
            return;
        }
        Client *NewClient = new Client(NewConnection, Address, MainStorage, Logs);
        std::thread *ClientThread = new std::thread(InitClient, NewClient);
    }
}

Client::Client(int InputSocket, struct sockaddr_in InputAddress, Storage& InputStorage, Logger& InputLogger) \
: MainStorage(InputStorage), Logs(InputLogger)
{
    Socket = InputSocket;
    Address = InputAddress;

    // std::thread(InitClient, this);
    // this -> ClientMainThread();
}

Client::~Client()
{
    close(Socket);
}

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
            MainStorage.Dump(Arguments[1]);
    }
    strcpy(buffer, (AnswerStr + " " + RetElem.Value + "\n").c_str());
    return 0;
}

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
}