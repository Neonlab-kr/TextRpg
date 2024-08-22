#include "HandlerObject.h"

HandlerObject::HandlerObject(SOCKET socket) : clientSocket(socket)
{
    commandMap["LoadItem"] = std::bind(&HandlerObject::loadItem, this);
    commandMap["SignIn"] = std::bind(&HandlerObject::signIn, this);
    commandMap["NewIDCheck"] = std::bind(&HandlerObject::newIDCheck, this);
    commandMap["SignUp"] = std::bind(&HandlerObject::signUp, this);
    commandMap["Transaction"] = std::bind(&HandlerObject::transaction, this);
    commandMap["LoadInventory"] = std::bind(&HandlerObject::loadInventory, this);
    commandMap["GenerateEnemy"] = std::bind(&HandlerObject::generateEnemy, this);
    commandMap["BattleRecord"] = std::bind(&HandlerObject::battleRecord, this);
    commandMap["LevelUp"] = std::bind(&HandlerObject::levelUp, this);
    commandMap["EquipItem"] = std::bind(&HandlerObject::equipItem, this);
    commandMap["ShopInit"] = std::bind(&HandlerObject::shopInit, this);
    commandMap["LoadShop"] = std::bind(&HandlerObject::loadShop, this);
    commandMap["UseItem"] = std::bind(&HandlerObject::useItem, this);
    commandMap["RankCheck"] = std::bind(&HandlerObject::rankCheck, this);
    commandMap["GetTradeList"] = std::bind(&HandlerObject::getTradeList, this);
    commandMap["GetMyTradeList"] = std::bind(&HandlerObject::getMyTradeList, this);
    commandMap["CancelTrade"] = std::bind(&HandlerObject::cancelTrade, this);
    commandMap["TradeSell"] = std::bind(&HandlerObject::tradeSell, this);
    commandMap["TradeBuy"] = std::bind(&HandlerObject::tradeBuy, this);
    commandMap["ReceivePayment"] = std::bind(&HandlerObject::receivePayment, this);
}

HandlerObject::~HandlerObject()
{
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
    }
}

void HandlerObject::Start()
{
    // Start handling the client in a separate thread
    HANDLE threadHandle = CreateThread(nullptr, 0, [](LPVOID param) -> DWORD {
        HandlerObject* handler = static_cast<HandlerObject*>(param);
        handler->HandleClient();
        return 0;
        }, this, 0, nullptr);

    if (threadHandle != nullptr)
    {
        CloseHandle(threadHandle);
    }
}

void HandlerObject::hSendMessage(const std::string& message)
{
    char buffer[5];
    send(clientSocket, message.c_str(), message.length(), 0);
    recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
}

std::string HandlerObject::ReceiveMessage()
{
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::string message = ".";
    send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    return "";
}

void HandlerObject::HandleClient()
{
    std::string message;
    while (true)
    {
        message = ReceiveMessage();
        if (message.empty())
        {
            break; // Client disconnected or error
        }
        std::map<std::string, std::function<void()>>::iterator it = commandMap.find(message);
        if (it != commandMap.end()) {
            std::cout << "Received: " << message << std::endl;
            it->second();
        }
        else
        {
            std::cout << "Unknown command: " << message << std::endl;
        }
    }
    std::cout << "Client disconnected" << std::endl;
    delete this;
}

void HandlerObject::loadItem()
{
    dbConn.executeQuery("SELECT * FROM ITEM");
    MYSQL_RES* result = dbConn.storeResult();
    hSendMessage(std::to_string(mysql_num_rows(result)));//아이템 수량 반환

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        hSendMessage(row[0]);//id
        hSendMessage(row[1]);//name
        hSendMessage(row[2]);//grade
        hSendMessage(row[3]);//price
        hSendMessage(row[4]);//type
        if (row[5]) hSendMessage(row[5]);//equiptype
        else hSendMessage("NULL");
        hSendMessage(row[6]);//atk
        hSendMessage(row[7]);//def
        hSendMessage(row[8]);//hp
        hSendMessage(row[9]);//dodgerate
        hSendMessage(row[10]);//criticalrate
    }
    std::cout << "Item data sent." << std::endl;
}

void HandlerObject::signIn()
{
    std::string id, pw;
    id = ReceiveMessage();
    pw = ReceiveMessage();

    dbConn.executeQuery("Select * from userinfo where ID = \'"+id+"\'");
    MYSQL_RES* result = dbConn.storeResult();
    MYSQL_ROW row;
    if (mysql_num_rows(result)==0)
    {
        //미가입된 id
        hSendMessage("NoID");
    }
    else
    {
        row = mysql_fetch_row(result);
        if (row[1] == pw)
        {
            //로그인 성공
            hSendMessage("Success");
            hSendMessage(row[2]);//job
            hSendMessage(row[3]);//level
            hSendMessage(row[4]);//exp
            hSendMessage(row[5]);//atk
            hSendMessage(row[6]);//def
            hSendMessage(row[7]);//hp
            hSendMessage(row[8]);//dodgerate
            hSendMessage(row[9]);//criticalrate
            hSendMessage(row[10]);//money
        }
        else
        {
            //맞지 않는 pw
            hSendMessage("Fail");
        }
    }
}

void HandlerObject::newIDCheck()
{
    std::string id = ReceiveMessage();
    dbConn.executeQuery("Select * from userinfo where ID = \'" + id + "\'");
    MYSQL_RES* result = dbConn.storeResult();
    
    if (mysql_num_rows(result) == 0)
    {
        //미가입된 id
        hSendMessage("New");
    }
    else
    {
        hSendMessage("Fail");
    }
}

void HandlerObject::signUp()
{
    std::string id, pw, job, atk, def, hp, dodgeRate, criticalRate;
    id = ReceiveMessage();
    pw = ReceiveMessage();
    job = ReceiveMessage();
    atk = ReceiveMessage();
    def = ReceiveMessage();
    hp = ReceiveMessage();
    dodgeRate = ReceiveMessage();
    criticalRate = ReceiveMessage();
    std::cout << id << ' ' << pw << ' ' << job << ' ' << atk << ' ' << def << ' ' << hp << ' ' << dodgeRate << ' ' << criticalRate << std::endl;
    dbConn.executeQuery("insert into userinfo(id,pw,job,atk,def,hp,dodgerate,criticalrate) values(\'" + id + "\',\'" + pw + "\',\'" + job + "\',\'" + atk + "\',\'" + def + "\',\'" + hp + "\',\'" + dodgeRate + "\',\'" + criticalRate + "\')");
}

void HandlerObject::shopInit()
{
    std::string id = ReceiveMessage();
    dbConn.executeQuery("insert into shop values(\'" + id + "\',\'1\',\'1\')");
    dbConn.executeQuery("insert into shop values(\'" + id + "\',\'2\',\'1\')");

    hSendMessage("2");
    
    hSendMessage("1");
    hSendMessage("1");

    hSendMessage("2");
    hSendMessage("1");

    std::cout << "Shop init sent." << std::endl;
}

void HandlerObject::loadShop()
{
    std::string id = ReceiveMessage();
    dbConn.executeQuery("select * from shop where shop_user_id = \'" + id + '\'');
    MYSQL_RES* result = dbConn.storeResult();
    hSendMessage(std::to_string(mysql_num_rows(result)));//아이템 수량 반환

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        hSendMessage(row[1]);//item_id
        hSendMessage(row[2]);//amount
    }
    std::cout << "Shop data sent." << std::endl;
}

void HandlerObject::transaction()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string type = ReceiveMessage();
    std::string price = ReceiveMessage();
    dbConn.executeQuery("insert into transaction(TRANS_USER_ID, TRANS_ITEM_ID,TRANS_TYPE,TRANS_PRICE) values(\'" + userId + "\',\'"+itemId + "\',\'"+type+ +"\',\'"+price+"\')");
    if (type == "SELL")
    {
        dbConn.executeQuery("update userinfo set MONEY = MONEY + \'" + price + "\' where id = \'" + userId + '\'');
    }
    else
    {
        dbConn.executeQuery("update userinfo set MONEY = MONEY - \'" + price + "\' where id = \'" + userId + '\'');
    }
}

void HandlerObject::loadInventory()
{
    std::string id = ReceiveMessage();
    dbConn.executeQuery("select * from inventory where USER_ID =\'" + id + '\'');
    MYSQL_RES* result = dbConn.storeResult();
    hSendMessage(std::to_string(mysql_num_rows(result)));//아이템 수량 반환

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        hSendMessage(row[1]);//item_id
        hSendMessage(row[2]);//amount
        if (row[3])
        {
            hSendMessage(row[3]);//equipped
        }
        else
        {
            hSendMessage("NULL");
        }
    }
    std::cout << "Inventory data sent." << std::endl;
}

void HandlerObject::generateEnemy()
{
    std::string location = ReceiveMessage();
    dbConn.executeQuery("select * from monster where LOCATION =\'" + location + '\'');
    MYSQL_RES* result = dbConn.storeResult();

    int typeCount = mysql_num_rows(result);
    srand(time(NULL));
    int sel = rand() % typeCount;
    mysql_data_seek(result, sel);

    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    hSendMessage(row[0]);//id
    hSendMessage(row[1]);//name
    hSendMessage(row[3]);//atk
    hSendMessage(row[4]);//def
    hSendMessage(row[5]);//dhp
    hSendMessage(row[6]);//dodgerate
    hSendMessage(row[7]);//criticalrate
    hSendMessage(row[8]);//rewardGold
    hSendMessage(row[9]);//rewardExp
    hSendMessage(row[10]);//rewardItem

    std::cout << "Monster generated." << std::endl;
}

void HandlerObject::battleRecord()
{
    std::string userId = ReceiveMessage();
    std::string monsterId = ReceiveMessage();
    std::string result = ReceiveMessage();
    dbConn.executeQuery("insert into battle(BATTLE_USER_ID, BATTLE_MONSTER_ID, RESULT) values(\'" + userId + "\', \'" + monsterId + "\', \'" + result + "\')");
}

void HandlerObject::levelUp()
{
    std::string userId = ReceiveMessage();
    int currentLevel = stoi(ReceiveMessage());
    int levelUpCount = stoi(ReceiveMessage());

    int expToDeduct = 0;
    for (int i = 0; i < levelUpCount; ++i) {
        expToDeduct += (currentLevel - levelUpCount + i) * 100;
    }

    dbConn.executeQuery("update userinfo set LEVEL = LEVEL + " + std::to_string(levelUpCount) + ", EXP = EXP - " + std::to_string(expToDeduct) + ", ATK = ATK + " + std::to_string(10* levelUpCount) + ", DEF = DEF + " + std::to_string(10 * levelUpCount) + ", HP = HP + " + std::to_string(50 * levelUpCount) +" WHERE ID = \'" + userId + '\'');
}

void HandlerObject::equipItem()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string equipped = ReceiveMessage();
    if (equipped == "0")
    {
        dbConn.executeQuery("update userinfo u join inventory i on u.ID = i.USER_ID join item it on i.ITEM_ID = it.ID set i.EQUIPPED = \'"+ equipped +"\', u.ATK = u.ATK - it.ATK, u.DEF = u.DEF - it.DEF, u.HP = u.HP - it.HP, u.DODGERATE = u.DODGERATE - it.DODGERATE, u.CRITICALRATE = u.CRITICALRATE - it.CRITICALRATE where u.ID = \'" + userId + "\' and i.ITEM_ID = \'" + itemId + '\'');
    }
    else
    {
        dbConn.executeQuery("update userinfo u join inventory i on u.ID = i.USER_ID join item it on i.ITEM_ID = it.ID set i.EQUIPPED = \'" + equipped + "\', u.ATK = u.ATK + it.ATK, u.DEF = u.DEF + it.DEF, u.HP = u.HP + it.HP, u.DODGERATE = u.DODGERATE + it.DODGERATE, u.CRITICALRATE = u.CRITICALRATE + it.CRITICALRATE where u.ID = \'" + userId + "\' and i.ITEM_ID = \'" + itemId + '\'');
    }
}

void HandlerObject::useItem()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string usedAll = ReceiveMessage();

    if (usedAll == "0")//다 안씀
    {
        dbConn.executeQuery("update inventory set AMOUNT = AMOUNT-1 where USER_ID = \'" + userId + "\' and ITEM_ID = \'" + itemId + '\'');
    }
    else//다 씀
    {
        dbConn.executeQuery("delete from inventory where USER_ID = \'" + userId + "\' and ITEM_ID = \'" + itemId + '\'');
    }
}

void HandlerObject::rankCheck()
{
    dbConn.executeQuery("select ID,LEVEL from userinfo order by LEVEL desc limit 10");
    MYSQL_RES* result = dbConn.storeResult();

    int count = mysql_num_rows(result);
    hSendMessage(std::to_string(count));
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        hSendMessage(row[0]);//id
        hSendMessage(row[1]);//level
    }
}

void HandlerObject::getTradeList()
{
    std::string type = ReceiveMessage();
    dbConn.executeQuery("select t.TRADE_ITEM_ID, t.PRICE from trade t join item i on t.TRADE_ITEM_ID = i.ID where i.TYPE = \'" + type + "\' and BUYER_ID is null");
    MYSQL_RES* result = dbConn.storeResult();

    int count = mysql_num_rows(result);
    hSendMessage(std::to_string(count));
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        hSendMessage(row[0]);//itemId
        hSendMessage(row[1]);//price
    }
}

void HandlerObject::getMyTradeList()
{
    std::string userId = ReceiveMessage();
    dbConn.executeQuery("select TRADE_ITEM_ID, PRICE from trade where SELLER_ID = \'" + userId + "\' and BUYER_ID is null");
    MYSQL_RES* result = dbConn.storeResult();

    int count = mysql_num_rows(result);
    hSendMessage(std::to_string(count));
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        hSendMessage(row[0]);//itemId
        hSendMessage(row[1]);//price
    }
}

void HandlerObject::cancelTrade()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string price = ReceiveMessage();

    dbConn.executeQuery("start transaction");
    dbConn.executeQuery("select ID from trade where SELLER_ID = \'" + userId + "\' and TRADE_ITEM_ID = \'" + itemId + "\' and PRICE = \'" + price + "\' and BUYER_ID is null limit 1 for update");
    MYSQL_RES* result = dbConn.storeResult();
    if (mysql_num_rows(result) > 0)
    {
        hSendMessage("Success");
        MYSQL_ROW row = mysql_fetch_row(result);

        dbConn.executeQuery("select AMOUNT from inventory where USER_ID = \'" + userId + "\'");
        dbConn.executeQuery("delete from trade where ID = \'" + (std::string)row[0] + "\'");
        dbConn.executeQuery("commit");
    }
    else
    {
        dbConn.executeQuery("commit");
        hSendMessage("Fail");
    }
}

void HandlerObject::tradeSell()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string price = ReceiveMessage();
    std::string soldAll = ReceiveMessage();

    dbConn.executeQuery("insert into trade(SELLER_ID,TRADE_ITEM_ID,PRICE) values(\'" + userId + "\',\'" + itemId + "\',\'" + price + "\')");

    if (soldAll == "0")
    {
        dbConn.executeQuery("update inventory set AMOUNT = AMOUNT-1 where USER_ID = \'"+userId+"\' and ITEM_ID = \'"+itemId+"\'");
    }
    else
    {
        dbConn.executeQuery("delete from inventory where USER_ID = \'" + userId + "\' and ITEM_ID = \'" + itemId + "\'");
    }
}

void HandlerObject::tradeBuy()
{
    std::string userId = ReceiveMessage();
    std::string itemId = ReceiveMessage();
    std::string price = ReceiveMessage();

    dbConn.executeQuery("start transaction");
    dbConn.executeQuery("select ID from trade where TRADE_ITEM_ID = \'" + itemId + "\' and PRICE = \'" + price + "\' and BUYER_ID is null limit 1 for update");
    MYSQL_RES* result = dbConn.storeResult();
    if (mysql_num_rows(result) > 0)
    {
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        std::string tradeId = row[0];
        dbConn.executeQuery("update trade set BUYER_ID = \'" + userId + "\' where ID = \'" + tradeId + "\'");
        dbConn.executeQuery("commit");
        hSendMessage("Success");

        dbConn.executeQuery("select * from inventory where USER_ID = \'" + userId + "\' and ITEM_ID = \'" + itemId + "\'");
        result = dbConn.storeResult();
        if (mysql_num_rows(result) > 0)
        {
            dbConn.executeQuery("update inventory set AMOUNT = AMOUNT + 1 where USER_ID = \'" + userId + "\' and ITEM_ID = \'" + itemId + "\'");
        }
        else
        {
            dbConn.executeQuery("insert into inventory values(\'"+ userId +"\', \'"+ itemId +"\', '1', '0') ");
        }
        dbConn.executeQuery("update userinfo set money = money - \'" + price + "\' where ID = \'" + userId + "\'");
    }
    else
    {
        dbConn.executeQuery("commit");
        hSendMessage("Fail");
    }
}

void HandlerObject::receivePayment()
{
    std::string userId = ReceiveMessage();
    dbConn.executeQuery("select SUM(PRICE) from trade where SELLER_ID = \'"+userId+"\' and BUYER_ID is not null and RECEIVED = 0");
    MYSQL_RES* result = dbConn.storeResult();
        MYSQL_ROW row = mysql_fetch_row(result);
    if (row[0]!=NULL)
    {
        hSendMessage(row[0]);
        dbConn.executeQuery("update userinfo set MONEY = MONEY + \'" + (std::string)row[0] + "\' where ID = \'"+userId+"\'");
        dbConn.executeQuery("update trade set RECEIVED = 1 where SELLER_ID = \'" + userId + "\' and BUYER_ID is not null");
    }
    else
    {
        hSendMessage("None");
    }
}
