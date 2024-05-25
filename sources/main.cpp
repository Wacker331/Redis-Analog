#include <iostream>
#include "logger.h"
#include "server.h"
#include "storage.h"

int main()
{
    Tree MainTree;
    Storage MainStorage(MainTree);
    Logger MainLogger("Logs");
    int port = 9001;
    Server MainServer(port, MainStorage, MainLogger);

}