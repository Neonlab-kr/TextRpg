#include "Shop.h"

Shop::Shop(ItemDB* itemdb)
{
    this->itemdb = itemdb;
}

void Shop::addItem(ShopItemStruct item)
{
    ItemType type = itemdb->getItem(item.itemID)->getItemType();
    switch (type)
    {
    case ItemType::장비:
        sellingItemList[(int)ItemType::장비].push_back(item);
        break;
    case ItemType::소비:
        sellingItemList[(int)ItemType::소비].push_back(item);
        break;
    case ItemType::기타:
        sellingItemList[(int)ItemType::기타].push_back(item);
        break;
    }
}

void Shop::showList(ItemType type) const
{
    int index = 1;
    for (ShopItemStruct item : sellingItemList[(int)type])
    {
        std::cout << '[' << index++ << "번 아이템]" << std::endl;
        itemdb->getItem((item.itemID))->printInfo();
        std::cout << "수량 : " << item.amount << std::endl;
        std::cout << std::endl;
    }
}

int Shop::getListSize(ItemType type) const
{
    return sellingItemList[(int)type].size();
}

int Shop::sellItem(ItemType type, int selection)
{
    sellingItemList[(int)type][selection - 1].amount--;
    int soldItem = sellingItemList[(int)type][selection - 1].itemID;

    if (sellingItemList[(int)type][selection - 1].amount == 0)
    {
        sellingItemList[(int)type].erase(sellingItemList[(int)type].begin() + (selection - 1));
    }

    return soldItem;
}

int Shop::getItemPrice(ItemType type, int selection) const
{
    return itemdb->getItem(sellingItemList[(int)type][selection - 1].itemID)->getPrice();
}

void Shop::buy(int id)
{
    ItemType type = itemdb->getItem(id)->getItemType();

    switch (type)
    {
    case ItemType::장비:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::장비])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::장비].push_back(ShopItemStruct(id, 1));
        break;

    case ItemType::소비:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::소비])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::소비].push_back(ShopItemStruct(id, 1));
        break;

    case ItemType::기타:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::기타])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::기타].push_back(ShopItemStruct(id, 1));
        break;
    }

}