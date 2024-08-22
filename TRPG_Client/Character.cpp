#include "Character.h"

using std::cout;
using std::endl;

#pragma region Character
void Character::printStatus() const
{
	cout << "���� : " << className << endl;
	cout << "���� : " << level << endl;
	cout << "����ġ : " << exp << " / " << level * 100 << endl;
	cout << "���ݷ� : " << atk << endl;
	cout << "���� : " << def << endl;
	cout << "ũ��Ƽ�� Ȯ�� : " << min(critRate,100) << "%" << endl;
	cout << "ȸ�� Ȯ�� : " << min(dodgeRate,100) << "%" << endl;
	cout << "���� ü�� : " << health << endl;
}

void Character::getStatus(int& pAtk, int& pDef, int& pHp, int& pDR, int& pCR) const
{
	pAtk = atk;
	pDef = def;
	pHp = health;
	pDR = dodgeRate;
	pCR = critRate;
}

void Character::setStatus(int pAtk, int pDef, int pHp, int pDR, int pCR)
{
	atk = pAtk;
	def = pDef;
	health = pHp;
	dodgeRate = pDR;
	critRate = pCR;
}

void Character::printInventory(ItemType type) const
{
	int index = 1;
	for (ItemStruct item : inventory[(int)type])
	{
		std::cout << '[' << index++ << "�� ������]" << std::endl;
		if (item.equipped)
		{
			std::cout << "(������)" << std::endl;
		}
		itemdb->getItem((item.itemID))->printInfo();
		std::cout << "���� : " << item.amount << std::endl;
		std::cout << std::endl;
	}
}

void Character::Defence(const int atkdamage, const int puredamage)
{
	if (atkdamage != 0 || puredamage != 0)
	{
		if (rateCheck(dodgeRate))
		{
			cout << "ȸ�ǿ� �����߽��ϴ�." << endl;
		}
		else
		{
			int damage;
			damage = (atkdamage - def);
			if (damage <= 0) damage = 0;
			damage += puredamage;
			cout << damage << "�� ���ظ� �Ծ����ϴ�." << endl;
			health -= damage;
		}
	}
}

int Character::getLevel() const
{
	return level;
}

int Character::getHealth() const
{
	return health;
}

int Character::getMoney() const
{
	return money;
}

int Character::getInventorySize(ItemType type) const
{
	return inventory[(int)type].size();
}

int Character::getItem(ItemType type, int sel)
{
	return inventory[(int)type][sel - 1].itemID;
}

void Character::addItem(ItemStruct item)
{
	ItemType type = itemdb->getItem(item.itemID)->getItemType();
	switch (type)
	{
	case ItemType::���:
		if (item.equipped)
		{
			equippedItem |= ((EquipItem*)itemdb->getItem(item.itemID))->getEquipType();
		}
		inventory[0].push_back(item);
		break;
	case ItemType::�Һ�:
		inventory[1].push_back(item);
		break;
	case ItemType::��Ÿ:
		inventory[2].push_back(item);
		break;
	}
}

void Character::gainReward(int money, int exp, int item)
{
	this->money += money;
	cout << money << "�� ��带 �����." << endl;
	Sleep(500);
	this->exp += exp;
	cout << exp << "�� ����ġ�� �����." << endl;
	Sleep(500);
	if(item != NULL)
	{
		ItemType type = itemdb->getItem(item)->getItemType();
		switch (type)
		{
		case ItemType::���:
		{
			std::vector<ItemStruct>::iterator iter = inventory[0].begin();
			for (iter = inventory[0].begin(); iter != inventory[0].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "��(��) �����." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[0].push_back(ItemStruct(item, 1, false));
			break;
		}
		case ItemType::�Һ�:
		{
			std::vector<ItemStruct>::iterator iter = inventory[1].begin();
			for (iter = inventory[1].begin(); iter != inventory[1].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "��(��) �����." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[1].push_back(ItemStruct(item, 1, false));
			break;
		}
		case ItemType::��Ÿ:
		{
			std::vector<ItemStruct>::iterator iter = inventory[2].begin();
			for (iter = inventory[2].begin(); iter != inventory[2].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "��(��) �����." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[2].push_back(ItemStruct(item, 1, false));
			break;
		}
		}
	cout << itemdb->getItem(item)->getName() << "��(��) �����." << endl;
	Sleep(500);
	}
}

void Character::loseExp()
{
	exp = exp - exp / 2;
}

int Character::levelUp()
{
	int levelUpCount = 0;
	while(exp >= level * 100)
	{
		levelUpCount++;
		exp -= level * 100;
		level++;
		atk += 10;
		def += 10;
		health += 50;
		cout << "Level UP!" << endl;
		cout << "���ݷ�, ����, ü���� 10�� �����մϴ�." << endl;
	}
	return levelUpCount;
}

int Character::sellItem(ItemType type,int selection)
{
	inventory[(int)type][selection - 1].amount--;
	int soldItem = inventory[(int)type][selection - 1].itemID;

	if (inventory[(int)type][selection - 1].amount == 0)
	{
		inventory[(int)type].erase(inventory[(int)type].begin() + (selection - 1));
	}
	money += itemdb->getItem(soldItem)->getPrice();
	cout << itemdb->getItem(soldItem)->getName() << "�Ǹ� �Ϸ�." << endl;
	cout << "���� ������ : " << money << endl;
	return soldItem;
}

void Character::buy(int id)
{
	ItemType type = itemdb->getItem(id)->getItemType();

	switch (type)
	{
	case ItemType::���:
		for (ItemStruct myItem : inventory[0])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "���ԿϷ�" << endl;
				cout << "���� ������ : " << money << endl;
				return;
			}
		}
		inventory[0].push_back(ItemStruct(id, 1, false));
		break;

	case ItemType::�Һ�:
		for (ItemStruct myItem : inventory[1])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "���ԿϷ�" << endl;
				cout << "���� ������ : " << money << endl;
				return;
			}
		}
		inventory[1].push_back(ItemStruct(id, 1, false));
		break;

	case ItemType::��Ÿ:
		for (ItemStruct myItem : inventory[2])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "���ԿϷ�" << endl;
				cout << "���� ������ : " << money << endl;
				return;
			}
		}
		inventory[2].push_back(ItemStruct(id, 1, false));
		break;
	}

	money -= itemdb->getItem(id)->getPrice();
	cout << itemdb->getItem(id)->getName() << "���ԿϷ�" << endl;
	cout << "���� ������ : " << money << endl;
}

bool Character::equippedCheck(EquipItem* equip)
{
	return (equippedItem & (equip->getEquipType())) != EquipType::NOTEQUIP;
}

bool Character::getEquipped(int sel)
{
	return inventory[0][sel - 1].equipped;
}

void Character::setEquipped(int sel, bool equipped)
{
	inventory[0][sel - 1].equipped = equipped;
}

void Character::equip(EquipItem* item)
{
	int itemAtk, itemDef, itemHp, itemDR, itemCR;
	item->getStatus(itemAtk, itemDef, itemHp, itemDR, itemCR);
	atk += itemAtk;
	def += itemDef;
	health += itemHp;
	dodgeRate += itemDR;
	critRate += itemCR;
	equippedItem |= item->getEquipType();
}

void Character::unequip(EquipItem* item)
{
	int itemAtk, itemDef, itemHp, itemDR, itemCR;
	item->getStatus(itemAtk, itemDef, itemHp, itemDR, itemCR);
	atk -= itemAtk;
	def -= itemDef;
	health -= itemHp;
	dodgeRate -= itemDR;
	critRate -= itemCR;
	equippedItem &= ~item->getEquipType();
}

int Character::tradeSellItem(ItemType type, int selection, bool& soldAll)
{
	inventory[(int)type][selection - 1].amount--;
	int soldItem = inventory[(int)type][selection - 1].itemID;

	if (inventory[(int)type][selection - 1].amount == 0)
	{
		soldAll = true;
		inventory[(int)type].erase(inventory[(int)type].begin() + (selection - 1));
	}
	else
	{
		soldAll = false;
	}
	cout << itemdb->getItem(soldItem)->getName() << " �Ǹ� ��� �Ϸ�." << endl;
	return soldItem;
}

void Character::tradeBuyItem(int id, int price)
{
	ItemType type = itemdb->getItem(id)->getItemType();
	bool notExist = true;

	for (std::vector<ItemStruct>::iterator iter = inventory[(int)type].begin(); iter != inventory[(int)type].end(); ++iter)
	{
		if (iter->itemID == id)
		{
			notExist = false;
			iter->amount++;
			break;
		}
	}

	if (notExist)
	{
		inventory[(int)type].push_back(ItemStruct(id, 1, false));
	}

	money -= price;
}

void Character::receivePayment(int money)
{
	this->money += money;
}

int Character::useConsumptionItem(int selection, bool& usedAll)
{
	ConsumptionItem* item = dynamic_cast<ConsumptionItem*>(itemdb->getItem(inventory[1][selection - 1].itemID));

	int usedItemId = inventory[(int)ItemType::�Һ�][selection - 1].itemID;
	inventory[(int)ItemType::�Һ�][selection - 1].amount--;

	if (inventory[(int)ItemType::�Һ�][selection - 1].amount == 0)
	{
		usedAll = true;
		inventory[(int)ItemType::�Һ�].erase(inventory[(int)ItemType::�Һ�].begin() + (selection - 1));
	}

	int itemAtk, itemDef, itemHp, itemDR, itemCR;
	item->getStatus(itemAtk, itemDef, itemHp, itemDR, itemCR);
	atk += itemAtk;
	def += itemDef;
	health += itemHp;
	dodgeRate += itemDR;
	critRate += itemCR;

	cout << itemdb->getItem(usedItemId)->getName() << "��� �Ϸ�." << endl;
	return usedItemId;	
}
#pragma endregion

#pragma region Dracula
void Dracula::bloodSucking(int& atkdamage)
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����! ���ݷ��� 1.5�踸ŭ�� �������� �����ϰ� ������ 50%�� ȸ���մϴ�." << endl;
		atkdamage = atk * 1.5;
		health += atkdamage * 0.5;
		cout << "���� : " << atkdamage << endl;
		cout << "ȸ���� : " << atkdamage * 0.5 << endl;
	}
	else
	{
		cout << "���ݷ� ��ŭ�� �������� �����ϰ� ������ 50%�� ȸ���մϴ�." << endl;
		atkdamage = atk;
		health += atkdamage * 0.5;
		cout << "���� : " << atkdamage << endl;
		cout << "ȸ���� : " << atkdamage * 0.5 << endl;
	}
}

void Dracula::hemophilia(int& atkdamage)
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����! ���ݷ��� 3�踸ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 3;
		cout << "���� ü�� : " << health / 20 << endl;
		health -= health / 20;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << "���� ü���� 5%�� �Ҹ��Ͽ� ���ݷ��� 2�� ��ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 2;
		cout << "���� ü�� : " << health / 20 << endl;
		health -= health / 20;
		cout << "���� : " << atkdamage << endl;
	}
}

Dracula::Dracula(ItemDB* itemdb)
{
	this->itemdb = itemdb;
	className = "Dracula";
	atk = 20;
	def = 7;
	critRate = 20;
	dodgeRate = 0;
	health = 120;
}

void Dracula::Act(int& atkdamage, int& pureDamage)
{
	while(true)
	{
		int sel;
		cout << "�̹� �Ͽ� �� �ൿ�� �����ϼ���" << endl;
		cout << "1. ���� ���� ��ų ���" << endl;
		cout << "2. ���纴 ��ų ���" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			cout << "\n";
			bloodSucking(atkdamage);
			return;

		case 2:
			cout << "\n";
			hemophilia(atkdamage);
			return;

		default:
			system("cls");
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
	}
}
#pragma endregion

#pragma region Elf
void Elf::piercingArrow(int& atkdamage, int& pureDamage) const
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����! ���ݷ��� 1.5�踸ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 0.75;
		pureDamage = atk * 0.75;
		cout << "�Ϲ� ���� : " << atkdamage << endl;
		cout << "���� ���� : " << pureDamage << endl;
	}
	else
	{
		cout << "���ݷ¸�ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 0.5;
		pureDamage = atk * 0.5;
		cout << "���� : " << atkdamage << endl;
		cout << "���� ���� : " << pureDamage << endl;
	}
}

void Elf::critBuff()
{
	cout << "ũ������ 5% �����մϴ�." << endl;
	critRate += 5;
}

void Elf::dodgeBuff()
{
	cout << "ȸ������ 5% �����մϴ�." << endl;
	dodgeRate += 5;
}

Elf::Elf(ItemDB* itemdb)
{
	this->itemdb = itemdb;
	className = "Elf";
	atk = 15;
	def = 7;
	critRate = 10;
	dodgeRate = 40;
	health = 80;
}

void Elf::Act(int& atkdamage, int& pureDamage)
{
	while (true)
	{
		int sel;
		cout << "�̹� �Ͽ� �� �ൿ�� �����ϼ���" << endl;
		cout << "1. ũ���� ���� ��ų ���" << endl;
		cout << "2. ȸ���� ���� ��ų ���" << endl;
		cout << "3. ���� ȭ�� ��ų ���" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			cout << "\n";
			critBuff();
			return;

		case 2:
			cout << "\n";
			dodgeBuff();
			return;

		case 3:
			cout << "\n";
			piercingArrow(atkdamage, pureDamage);
			return;

		default:
			system("cls");
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
	}
}
#pragma endregion

#pragma region Human
void Human::atkBuff()
{
	cout << "���ݷ��� 5 �����մϴ�." << endl;
	atk += 5;
}

void Human::defBuff()
{
	cout << "������ 5 �����մϴ�." << endl;
	def += 5;
}

void Human::heal()
{
	cout << "���ݷ��� ���ݸ�ŭ ü���� ȸ���մϴ�." << endl;
	health += atk;
}

void Human::normalAttack(int& atkdamage) const
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����! ���ݷ��� 1.5�踸ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 1.5;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << "���ݷ¸�ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk;
		cout << "���� : " << atkdamage << endl;
	}
}

Human::Human(ItemDB* itemdb)
{
	this->itemdb = itemdb;
	className = "Human";
	atk = 10;
	def = 5;
	critRate = 20;
	dodgeRate = 20;
	health = 100;
}

void Human::Act(int& atkdamage, int& pureDamage)
{
	while(true)
	{
		int sel;
		cout << "�̹� �Ͽ� �� �ൿ�� �����ϼ���" << endl;
		cout << "1. ���ݷ� ���� ��ų ���" << endl;
		cout << "2. ���� ���� ��ų ���" << endl;
		cout << "3. ȸ�� ��ų ���" << endl;
		cout << "4. �Ϲ� ���� ��ų ���" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			cout << "\n";
			atkBuff();
			return;

		case 2:
			cout << "\n";
			defBuff();
			return;

		case 3:
			cout << "\n";
			heal();
			return;

		case 4:
			cout << "\n";
			normalAttack(atkdamage);
			return;

		default:
			system("cls");
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
	}
}
#pragma endregion

#pragma region Orc
void Orc::normalAttack(int& atkdamage) const
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����! ���ݷ��� 4�踸ŭ�� �������� �����մϴ�." << endl;
		atkdamage = atk * 4;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << "���ݷ��� 2�� �������� �����մϴ�." << endl;
		atkdamage = atk * 2;
		cout << "���� : " << atkdamage << endl;
	}
}

Orc::Orc(ItemDB* itemdb)
{
	this->itemdb = itemdb;
	className = "Orc";
	atk = 20;
	def = 10;
	critRate = 30;
	dodgeRate = 0;
	health = 150;
}

void Orc::Act(int& atkdamage, int& pureDamage)
{
	while(true)
	{
		int sel;
		cout << "�̹� �Ͽ� �� �ൿ�� �����ϼ���" << endl;
		cout << "1. �Ϲ� ���� ��ų ���" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			cout << "\n";
			normalAttack(atkdamage);
			return;

		default:
			system("cls");
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
	}
}
#pragma endregion