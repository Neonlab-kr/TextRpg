#include "ItemDB.h"

ItemDB::~ItemDB()
{
	for (std::pair<const int, Item*>& itemPair : itemDBMap)
	{
		delete itemPair.second;
	}
}

Item* ItemDB::getItem(int id)
{
	std::map<int, Item*>::iterator iter = itemDBMap.find(id);
	if (iter != itemDBMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void ItemDB::addItem(int id,Item* item)
{
	itemDBMap[id] = item;
}
