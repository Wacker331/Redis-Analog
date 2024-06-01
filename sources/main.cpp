#include <iostream>
#include "logger.h"
#include "server.h"
#include "storage.h"

int main(int argc, char *argv[])
{
    Tree MainTree;
    Storage MainStorage(MainTree);
    std::string LogFileName = "Logs";
    int port = 9000;
    int MaxClients = 10;

    for(int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            std::cout << "Usage: " << argv[0];
            std::cout << " [-f, --file <dump filename>] [-p, --port <port>] "\
                            "[-m, --max_clients <number of clients]" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
        {
            if ((i + 1) < argc)
                MainStorage.ReadDump(argv[i + 1]);
            else
                std::cout << "-f flag provided, but filename of dump file wasn't" << std::endl;
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0)
        {
            if ((i + 1) < argc)
                port = atoi(argv[i + 1]);
            else
                std::cout << "-p flag provided, but port number wasn't" << std::endl;
        }
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max_clients") == 0)
        {
            if ((i + 1) < argc)
                MaxClients = atoi(argv[i + 1]);
            else
                std::cout << "-m flag provided, but maximum clients number wasn't" << std::endl;
        }
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--logs") == 0)
        {
            if ((i + 1) < argc)
                LogFileName = argv[i + 1];
            else
                std::cout << "-l flag provided, but filename for logs wasn't" << std::endl;
        }
    }
    Logger MainLogger(LogFileName);
    std::thread *LoggerThread = new std::thread(InitLogger, &MainLogger);
    Server MainServer(port, MainStorage, MainLogger, MaxClients);
    return 0;
}