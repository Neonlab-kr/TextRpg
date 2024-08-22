#pragma once
#include "ItemDB.h"
#include <vector>

struct TradeShopItemStruct
{
	int itemID;
	int price;

	TradeShopItemStruct(int id, int price) :itemID(id), price(price) {}
};

class TradeShop
{
private:
	ItemDB* itemdb;
	std::vector<TradeShopItemStruct> tradeItemList;

public:
	TradeShop(ItemDB* itemdb);
	void clearList();
	void addItem(TradeShopItemStruct item);
	void showList() const;
	int getListSize() const;
	int getItemId(int sel) const;
	int getItemPrice(int sel) const;
	void sellItem(int sel);
};