#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"

Client::Client(const std::string& address, int port) : address(address), port(port), clientSocket(INVALID_SOCKET) {}

Client::~Client() {
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
        WSACleanup();
    }
}

void Client::Connect() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        exit(0);
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        exit(0);
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(address.c_str()); // Use inet_addr here

    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(0);
        return;
    }
}

void Client::Send(const std::string& message) {
    char buffer[5];
    send(clientSocket, message.c_str(), message.length(), 0);
    recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
}

std::string Client::Receive() {
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::string message = ".";
    send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    return "";
}
