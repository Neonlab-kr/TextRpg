#pragma once
#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    Client(const std::string& address, int port);
    ~Client();
    void Connect();
    void Send(const std::string& message);
    std::string Receive();
private:
    std::string address;
    int port;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
};