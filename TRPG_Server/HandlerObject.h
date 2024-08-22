#pragma once

#include <winsock2.h>
#include <string>
#include <iostream>
#include <map>
#include <functional>

#include "DBconn.h"

#pragma comment(lib, "ws2_32.lib")

class HandlerObject {
private:
    SOCKET clientSocket;
    DBconn dbConn;

    std::map<std::string, std::function<void()>> commandMap;

    void hSendMessage(const std::string& message);
    std::string ReceiveMessage();

    void HandleClient();

    void loadItem();

    void signIn();
    void newIDCheck();
    void signUp();

    void shopInit();
    void loadShop();

    void loadInventory();

    void generateEnemy();
    void battleRecord();
    void levelUp();

    void transaction();

    void equipItem();
    void useItem();

    void rankCheck();

    void getTradeList();
    void getMyTradeList();
    void cancelTrade();
    void tradeSell();
    void tradeBuy();
    void receivePayment();
public:
    HandlerObject(SOCKET clientSocket);
    ~HandlerObject();
    void Start();
};
