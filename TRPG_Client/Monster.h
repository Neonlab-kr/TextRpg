#pragma once
#include <string>
#include <time.h>
#include "Utils.h"

class Monster
{
protected:
	int id;
	std::string name;
	int atk;
	int def;
	int critRate;
	int dodgeRate;
	int health;
	int rewardMoney;
	int rewardExp;
	int rewardItemID;
public:
	virtual ~Monster() {};
	Monster(int id, std::string name, int atk, int def, int critRate, int dodgeRate, int health, int rewardMoney, int rewardExp, int rewardItemID)
		: id(id), name(name), atk(atk), def(def), critRate(critRate), dodgeRate(dodgeRate), health(health), rewardMoney(rewardMoney), rewardExp(rewardExp), rewardItemID(rewardItemID) {}
	void printInfo() const;
	void TakeDamage(int atkDamage,int  pureDamage);
	int getID() const;
	int getHealth() const;
	void reward(int& money, int& exp, int& item) const;
	virtual void Act(int& atkdamage, int& pureDamage) = 0;
};

class EasyMonster :public Monster
{
private:
	void normalAttack(int& atkdamage);

public:
	EasyMonster(int id, std::string name, int atk, int def, int critRate, int dodgeRate, int health, int rewardMoney, int rewardExp, int rewardItemID)
		: Monster(id, name, atk, def, critRate, dodgeRate, health, rewardMoney, rewardExp, rewardItemID) {}
	~EasyMonster() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};

class NormalMonster :public Monster
{
private:
	void normalAttack(int& atkdamage);
	void strongAttack(int& atkdamage);

public:
	NormalMonster(int id, std::string name, int atk, int def, int critRate, int dodgeRate, int health, int rewardMoney, int rewardExp, int rewardItemID)
		: Monster(id, name, atk, def, critRate, dodgeRate, health, rewardMoney, rewardExp, rewardItemID) {}
	~NormalMonster() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};

class HardMonster :public Monster
{
private:
	void normalAttack(int& atkdamage, int& pureDamage);
	void strongAttack(int& atkdamage, int& pureDamage);
	void specialAttack(int& atkdamage, int& pureDamage);

public:
	HardMonster(int id, std::string name, int atk, int def, int critRate, int dodgeRate, int health, int rewardMoney, int rewardExp, int rewardItemID)
		: Monster(id, name, atk, def, critRate, dodgeRate, health, rewardMoney, rewardExp, rewardItemID) {}
	~HardMonster() override {}
	void Act(int& atkdamage, int& pureDamage) override;
};