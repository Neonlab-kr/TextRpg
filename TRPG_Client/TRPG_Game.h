#pragma once
#include "Client.h"
#include "ItemDB.h"
#include "Shop.h"
#include "TradeShop.h"
#include "Character.h"
#include "Monster.h"


class TRPG_Game
{
private:
	std::string userId;
	ItemDB itemdb;
	Shop* shop;
	TradeShop* tradeShop;
	Character* player;
	Monster* monster;
	Client* client;


	void connectServer();
	void loadItemDB();
	void printStartScreen();
	void printRules();

	void signScreen();

	void signIn();
	void signUp();
	bool newIDCheck(std::string& id);
	void selClass(std::string& pw);
	void serverSignUp(std::string& pw, const std::string& job);

	void shopInit();
	void loadShop();
	void loadPlayerInven();
	void loadTradeList(const std::string& type);
	void loadMyTradeList(std::string userId);

	void mainMenu();

	void shopMenu();
	void shopMenu_buyItemMenu();
	void shopMenu_buyItem(ItemType type);
	void shopMenu_sellItemMenu();
	void shopMenu_sellItem(ItemType type);

	void inventoryMenu();
	void inventoryMenu_showItem();
	void inventoryMenu_equipItem();
	void inventoryMenu_unequipItem();

	void fight();
	void chooseLocation(std::string& location);
	void generateEnemy(const std::string& location);
	void playerTurn();
	void useConsumptionItem();
	bool monsterDead();
	void monsterTurn();
	bool playerDead();

	void tradeMenu();
	void tradeMenu_buyItemMenu();
	void tradeMenu_buyItem();
	void tradeMenu_sellItemMenu();
	void tradeMenu_sellItem(ItemType type);
	void tradeMenu_cancelTrade();
	void tradeMenu_receivePayment();

	void rankCheck();
public:
	TRPG_Game();
	~TRPG_Game();
	void gameStart();
};