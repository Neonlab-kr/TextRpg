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
    case ItemType::���:
        sellingItemList[(int)ItemType::���].push_back(item);
        break;
    case ItemType::�Һ�:
        sellingItemList[(int)ItemType::�Һ�].push_back(item);
        break;
    case ItemType::��Ÿ:
        sellingItemList[(int)ItemType::��Ÿ].push_back(item);
        break;
    }
}

void Shop::showList(ItemType type) const
{
    int index = 1;
    for (ShopItemStruct item : sellingItemList[(int)type])
    {
        std::cout << '[' << index++ << "�� ������]" << std::endl;
        itemdb->getItem((item.itemID))->printInfo();
        std::cout << "���� : " << item.amount << std::endl;
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
    case ItemType::���:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::���])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::���].push_back(ShopItemStruct(id, 1));
        break;

    case ItemType::�Һ�:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::�Һ�])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::�Һ�].push_back(ShopItemStruct(id, 1));
        break;

    case ItemType::��Ÿ:
        for (ShopItemStruct myItem : sellingItemList[(int)ItemType::��Ÿ])
        {
            if (myItem.itemID == id)
            {
                myItem.amount++;
                return;
            }
        }
        sellingItemList[(int)ItemType::��Ÿ].push_back(ShopItemStruct(id, 1));
        break;
    }

}