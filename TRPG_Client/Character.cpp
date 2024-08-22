#include "Character.h"

using std::cout;
using std::endl;

#pragma region Character
void Character::printStatus() const
{
	cout << "종족 : " << className << endl;
	cout << "레벨 : " << level << endl;
	cout << "경험치 : " << exp << " / " << level * 100 << endl;
	cout << "공격력 : " << atk << endl;
	cout << "방어력 : " << def << endl;
	cout << "크리티컬 확률 : " << min(critRate,100) << "%" << endl;
	cout << "회피 확률 : " << min(dodgeRate,100) << "%" << endl;
	cout << "현재 체력 : " << health << endl;
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
		std::cout << '[' << index++ << "번 아이템]" << std::endl;
		if (item.equipped)
		{
			std::cout << "(착용중)" << std::endl;
		}
		itemdb->getItem((item.itemID))->printInfo();
		std::cout << "수량 : " << item.amount << std::endl;
		std::cout << std::endl;
	}
}

void Character::Defence(const int atkdamage, const int puredamage)
{
	if (atkdamage != 0 || puredamage != 0)
	{
		if (rateCheck(dodgeRate))
		{
			cout << "회피에 성공했습니다." << endl;
		}
		else
		{
			int damage;
			damage = (atkdamage - def);
			if (damage <= 0) damage = 0;
			damage += puredamage;
			cout << damage << "의 피해를 입었습니다." << endl;
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
	case ItemType::장비:
		if (item.equipped)
		{
			equippedItem |= ((EquipItem*)itemdb->getItem(item.itemID))->getEquipType();
		}
		inventory[0].push_back(item);
		break;
	case ItemType::소비:
		inventory[1].push_back(item);
		break;
	case ItemType::기타:
		inventory[2].push_back(item);
		break;
	}
}

void Character::gainReward(int money, int exp, int item)
{
	this->money += money;
	cout << money << "의 골드를 얻었다." << endl;
	Sleep(500);
	this->exp += exp;
	cout << exp << "의 경험치를 얻었다." << endl;
	Sleep(500);
	if(item != NULL)
	{
		ItemType type = itemdb->getItem(item)->getItemType();
		switch (type)
		{
		case ItemType::장비:
		{
			std::vector<ItemStruct>::iterator iter = inventory[0].begin();
			for (iter = inventory[0].begin(); iter != inventory[0].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "을(를) 얻었다." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[0].push_back(ItemStruct(item, 1, false));
			break;
		}
		case ItemType::소비:
		{
			std::vector<ItemStruct>::iterator iter = inventory[1].begin();
			for (iter = inventory[1].begin(); iter != inventory[1].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "을(를) 얻었다." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[1].push_back(ItemStruct(item, 1, false));
			break;
		}
		case ItemType::기타:
		{
			std::vector<ItemStruct>::iterator iter = inventory[2].begin();
			for (iter = inventory[2].begin(); iter != inventory[2].end(); iter++)
			{
				if (iter->itemID == item)
				{
					iter->amount = iter->amount + 1;
					cout << itemdb->getItem(item)->getName() << "을(를) 얻었다." << endl;
					Sleep(500);
					return;
				}
			}
			inventory[2].push_back(ItemStruct(item, 1, false));
			break;
		}
		}
	cout << itemdb->getItem(item)->getName() << "을(를) 얻었다." << endl;
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
		cout << "공격력, 방어력, 체력이 10씩 증가합니다." << endl;
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
	cout << itemdb->getItem(soldItem)->getName() << "판매 완료." << endl;
	cout << "현재 소지금 : " << money << endl;
	return soldItem;
}

void Character::buy(int id)
{
	ItemType type = itemdb->getItem(id)->getItemType();

	switch (type)
	{
	case ItemType::장비:
		for (ItemStruct myItem : inventory[0])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "구입완료" << endl;
				cout << "남은 소지금 : " << money << endl;
				return;
			}
		}
		inventory[0].push_back(ItemStruct(id, 1, false));
		break;

	case ItemType::소비:
		for (ItemStruct myItem : inventory[1])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "구입완료" << endl;
				cout << "남은 소지금 : " << money << endl;
				return;
			}
		}
		inventory[1].push_back(ItemStruct(id, 1, false));
		break;

	case ItemType::기타:
		for (ItemStruct myItem : inventory[2])
		{
			if (myItem.itemID == id)
			{
				myItem.amount++;
				money -= itemdb->getItem(id)->getPrice();
				cout << itemdb->getItem(id)->getName() << "구입완료" << endl;
				cout << "남은 소지금 : " << money << endl;
				return;
			}
		}
		inventory[2].push_back(ItemStruct(id, 1, false));
		break;
	}

	money -= itemdb->getItem(id)->getPrice();
	cout << itemdb->getItem(id)->getName() << "구입완료" << endl;
	cout << "남은 소지금 : " << money << endl;
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
	cout << itemdb->getItem(soldItem)->getName() << " 판매 등록 완료." << endl;
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

	int usedItemId = inventory[(int)ItemType::소비][selection - 1].itemID;
	inventory[(int)ItemType::소비][selection - 1].amount--;

	if (inventory[(int)ItemType::소비][selection - 1].amount == 0)
	{
		usedAll = true;
		inventory[(int)ItemType::소비].erase(inventory[(int)ItemType::소비].begin() + (selection - 1));
	}

	int itemAtk, itemDef, itemHp, itemDR, itemCR;
	item->getStatus(itemAtk, itemDef, itemHp, itemDR, itemCR);
	atk += itemAtk;
	def += itemDef;
	health += itemHp;
	dodgeRate += itemDR;
	critRate += itemCR;

	cout << itemdb->getItem(usedItemId)->getName() << "사용 완료." << endl;
	return usedItemId;	
}
#pragma endregion

#pragma region Dracula
void Dracula::bloodSucking(int& atkdamage)
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중! 공격력의 1.5배만큼의 위력으로 공격하고 위력의 50%를 회복합니다." << endl;
		atkdamage = atk * 1.5;
		health += atkdamage * 0.5;
		cout << "위력 : " << atkdamage << endl;
		cout << "회복량 : " << atkdamage * 0.5 << endl;
	}
	else
	{
		cout << "공격력 만큼의 위력으로 공격하고 위력의 50%를 회복합니다." << endl;
		atkdamage = atk;
		health += atkdamage * 0.5;
		cout << "위력 : " << atkdamage << endl;
		cout << "회복량 : " << atkdamage * 0.5 << endl;
	}
}

void Dracula::hemophilia(int& atkdamage)
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중! 공격력의 3배만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 3;
		cout << "잃은 체력 : " << health / 20 << endl;
		health -= health / 20;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << "현재 체력의 5%를 소모하여 공격력의 2배 만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 2;
		cout << "잃은 체력 : " << health / 20 << endl;
		health -= health / 20;
		cout << "위력 : " << atkdamage << endl;
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
		cout << "이번 턴에 할 행동을 선택하세요" << endl;
		cout << "1. 흡혈 공격 스킬 사용" << endl;
		cout << "2. 혈사병 스킬 사용" << endl;

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
			cout << "잘못된 입력입니다." << endl;
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
		cout << "치명타 적중! 공격력의 1.5배만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 0.75;
		pureDamage = atk * 0.75;
		cout << "일반 위력 : " << atkdamage << endl;
		cout << "고정 피해 : " << pureDamage << endl;
	}
	else
	{
		cout << "공격력만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 0.5;
		pureDamage = atk * 0.5;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정 피해 : " << pureDamage << endl;
	}
}

void Elf::critBuff()
{
	cout << "크리율이 5% 증가합니다." << endl;
	critRate += 5;
}

void Elf::dodgeBuff()
{
	cout << "회피율이 5% 증가합니다." << endl;
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
		cout << "이번 턴에 할 행동을 선택하세요" << endl;
		cout << "1. 크리율 증가 스킬 사용" << endl;
		cout << "2. 회피율 증가 스킬 사용" << endl;
		cout << "3. 관통 화살 스킬 사용" << endl;

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
			cout << "잘못된 입력입니다." << endl;
			break;
		}
	}
}
#pragma endregion

#pragma region Human
void Human::atkBuff()
{
	cout << "공격력이 5 증가합니다." << endl;
	atk += 5;
}

void Human::defBuff()
{
	cout << "방어력이 5 증가합니다." << endl;
	def += 5;
}

void Human::heal()
{
	cout << "공격력의 절반만큼 체력을 회복합니다." << endl;
	health += atk;
}

void Human::normalAttack(int& atkdamage) const
{
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중! 공격력의 1.5배만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 1.5;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << "공격력만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk;
		cout << "위력 : " << atkdamage << endl;
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
		cout << "이번 턴에 할 행동을 선택하세요" << endl;
		cout << "1. 공격력 증가 스킬 사용" << endl;
		cout << "2. 방어력 증가 스킬 사용" << endl;
		cout << "3. 회복 스킬 사용" << endl;
		cout << "4. 일반 공격 스킬 사용" << endl;

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
			cout << "잘못된 입력입니다." << endl;
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
		cout << "치명타 적중! 공격력의 4배만큼의 위력으로 공격합니다." << endl;
		atkdamage = atk * 4;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << "공격력의 2배 위력으로 공격합니다." << endl;
		atkdamage = atk * 2;
		cout << "위력 : " << atkdamage << endl;
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
		cout << "이번 턴에 할 행동을 선택하세요" << endl;
		cout << "1. 일반 공격 스킬 사용" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			cout << "\n";
			normalAttack(atkdamage);
			return;

		default:
			system("cls");
			cout << "잘못된 입력입니다." << endl;
			break;
		}
	}
}
#pragma endregion