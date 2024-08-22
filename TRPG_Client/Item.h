#pragma once
#include<iostream>
#include "Utils.h"

enum class Grade
{
	COMMON,RARE,EPIC,LEGENDARY,NONE
};

enum class ItemType
{
	장비, 소비, 기타
};

enum class EquipType
{
	NOTEQUIP = 0, 투구 = 1, 상의 = 2, 하의 = 4, 장갑 = 8, 신발 = 16, 무기 = 32
};

inline EquipType operator|(EquipType lhs, EquipType rhs)
{
	return static_cast<EquipType>(
		static_cast<std::underlying_type<EquipType>::type>(lhs) |
		static_cast<std::underlying_type<EquipType>::type>(rhs)
		);
}

inline EquipType& operator|=(EquipType& lhs, EquipType rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

inline EquipType operator&(EquipType lhs, EquipType rhs)
{
	return static_cast<EquipType>(
		static_cast<std::underlying_type<EquipType>::type>(lhs) &
		static_cast<std::underlying_type<EquipType>::type>(rhs)
		);
}

inline EquipType& operator&=(EquipType& lhs, EquipType rhs)
{
	lhs = lhs & rhs;
	return lhs;
}

inline EquipType operator~(EquipType rhs)
{
	return static_cast<EquipType>(
		~static_cast<std::underlying_type<EquipType>::type>(rhs)
		);
}

class Item abstract
{
protected:
	Grade grade;
	ItemType type;
	const std::string name;
	int price;
public:
	Item(Grade grade, ItemType type, const std::string& name, int price) : grade(grade), type(type), name(name), price(price) {};
	virtual ~Item() {}
	ItemType getItemType() const;
	std::string getName() const;
	int getPrice() const;
	void setPrice(int price);
	virtual void printInfo() = 0;
};

class ConsumptionItem : public Item
{
private:
	const int atk, def, hp, dodgeRate, criticalRate;
public:
	ConsumptionItem(Grade grade, const std::string& name, int price, int atk, int def, int hp, int dodgeRate, int criticalRate);
	void getStatus(int& itemAtk, int& itemDef, int& itemHp, int& itemDR, int& itemCR) const;
	~ConsumptionItem() override {}
	void printInfo() override;
};

class EquipItem : public Item
{
private:
	EquipType equipType;
	const int atk, def, hp, dodgeRate, criticalRate;
public:
	EquipItem(Grade grade, const std::string& name, int price, EquipType equipType, int atk, int def, int hp, int dodgeRate, int criticalRate);
	~EquipItem() override {}
	void printInfo() override;
	void getStatus(int& itemAtk, int& itemDef, int& itemHp, int& itemDR, int& itemCR) const;
	EquipType getEquipType() const;
};

class OtherItem : public Item
{
public:
	OtherItem(Grade grade, const std::string& name, int price);
	~OtherItem() override {}
	void printInfo() override;
};