#pragma once
#include <iostream>
#include<vector>
#include<map>
#include "Item.h"
#include "ItemDB.h"

struct ShopItemStruct
{
	int itemID;
	int amount;

	ShopItemStruct(int id, int amount) :itemID(id), amount(amount) {}
};

class Shop
{
private:
	ItemDB* itemdb;
	std::vector<ShopItemStruct> sellingItemList[3];//equip,consumption,other

public:
	Shop(ItemDB* itemdb);
	void addItem(ShopItemStruct item);
	void showList(ItemType type) const;
	int getListSize(ItemType type) const;
	int sellItem(ItemType type, int selection);
	int getItemPrice(ItemType type, int selection) const;
	void buy(int id);
};