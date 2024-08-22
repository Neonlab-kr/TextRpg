#pragma once
#include <conio.h>
#include <vector>
#include "Item.h"
#include "ItemDB.h"
#include "Utils.h"

struct ItemStruct
{
	int itemID;
	int amount;
	bool equipped;

	ItemStruct(int id, int amount, bool equipped) :itemID(id), amount(amount), equipped(equipped) {}
};

class Character abstract
{
protected:
	ItemDB* itemdb;
	std::vector<ItemStruct> inventory[3];//equip,consumption,other
	std::string className;
	int level = 1;
	int exp = 0;
	int money = 0;
	int atk;
	int def;
	int critRate;
	int dodgeRate;
	int health;
	EquipType equippedItem = EquipType::NOTEQUIP;

public:
	int getLevel() const;
	int getHealth() const;
	int getMoney() const;

	void printStatus() const;
	virtual void getStatus(int& pAtk, int& pDef, int& pHp, int& pDR, int& pCR) const;
	virtual void setStatus(int pAtk, int pDef, int pHp, int pDR, int pCR);

	void addItem(ItemStruct item);
	void printInventory(ItemType type) const;
	int getInventorySize(ItemType type) const;
	int getItem(ItemType type, int sel);
	int sellItem(ItemType type,int selection);
	void buy(int id);

	bool equippedCheck(EquipItem* equip);
	bool getEquipped(int sel);
	void setEquipped(int sel,bool equipped);
	void equip(EquipItem* item);
	void unequip(EquipItem* item);

	int tradeSellItem(ItemType type, int selection, bool& soldAll);
	void tradeBuyItem(int id, int price);
	void receivePayment(int money);

	void Defence(const int atkdamage, const int puredamage);
	int useConsumptionItem(int selection, bool& usedAll);
	void gainReward(int money, int exp, int item);
	void loseExp();
	int levelUp();

	Character() = default;
	Character(std::string className,ItemDB* itemdb,int level, int exp, int atk, int def, int hp, int dodgeRate, int criticalRate, int money)
		:className(className),itemdb(itemdb),level(level), exp(exp), atk(atk), def(def), health(hp), dodgeRate(dodgeRate), critRate(criticalRate), money(money) {}
	virtual ~Character() {};
	virtual void Act(int& atkdamage, int& pureDamage) = 0;
};

class Dracula : public Character
{
private:
	void bloodSucking(int& atkdamage);
	void hemophilia(int& atkdamage);

public:
	Dracula(ItemDB* itemDB);
	Dracula(ItemDB* itemdb, int level, int exp, int atk, int def, int hp, int dodgeRate, int criticalRate, int money) : Character("Dracula", itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money) {}
	~Dracula() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};

class Elf : public Character
{
private:
	void piercingArrow(int& atkdamage, int& pureDamage) const;
	void critBuff();
	void dodgeBuff();

public:
	Elf(ItemDB* itemDB);
	Elf(ItemDB* itemdb, int level, int exp, int atk, int def, int hp, int dodgeRate, int criticalRate, int money) : Character("Elf", itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money) {}
	~Elf() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};

class Human : public Character
{
private:
	void atkBuff();
	void defBuff();
	void heal();
	void normalAttack(int& atkdamage) const;

public:
	Human(ItemDB* itemDB);
	Human(ItemDB* itemdb, int level, int exp, int atk, int def, int hp, int dodgeRate, int criticalRate, int money) : Character("Human",itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money) {}
	~Human() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};

class Orc : public Character
{
private:
	void normalAttack(int& atkdamage) const;

public:
	Orc(ItemDB* itemDB);
	Orc(ItemDB* itemdb, int level, int exp, int atk, int def, int hp, int dodgeRate, int criticalRate, int money) : Character("Orc", itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money) {}
	~Orc() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};