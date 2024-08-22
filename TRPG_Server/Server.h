#pragma once

#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include "HandlerObject.h"

#pragma comment(lib, "ws2_32.lib")

class Server {
private:
    int port;
    SOCKET serverSocket;
    SOCKADDR_IN serverAddr;
    void AcceptClients();

public:
    Server(int port);
    ~Server();
    void Start();
};
