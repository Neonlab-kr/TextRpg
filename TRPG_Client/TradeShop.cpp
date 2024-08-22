#include "TradeShop.h"

TradeShop::TradeShop(ItemDB* itemdb)
{
	this->itemdb = itemdb;
}

void TradeShop::clearList()
{
    tradeItemList.clear();
}

void TradeShop::addItem(TradeShopItemStruct item)
{
    tradeItemList.push_back(item);
}

void TradeShop::showList() const
{
    int index = 1;
    for (TradeShopItemStruct item : tradeItemList)
    {
        std::cout << '[' << index++ << "번 아이템]" << std::endl;
        itemdb->getItem((item.itemID))->printInfo();
        std::cout << "가격 : " << item.price << std::endl;
        std::cout << std::endl;
    }
}

int TradeShop::getListSize() const
{
    return tradeItemList.size();
}

int TradeShop::getItemId(int sel) const
{
    return tradeItemList[sel - 1].itemID;
}

int TradeShop::getItemPrice(int sel) const
{
    return tradeItemList[sel - 1].price;
}

void TradeShop::sellItem(int sel)
{
    tradeItemList.erase(tradeItemList.begin() + (sel - 1));
}