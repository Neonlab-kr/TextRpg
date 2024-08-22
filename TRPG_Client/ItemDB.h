#pragma once
#include"Item.h"
#include <map>

class ItemDB
{
private:
	std::map<int,Item*> itemDBMap;

public:
	~ItemDB();
	Item* getItem(int id);
	void addItem(int id,Item* item);
};

