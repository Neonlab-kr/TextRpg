#include "Item.h"

using std::cout;
using std::endl;

#pragma region ConsumptionItem
ConsumptionItem::ConsumptionItem(Grade grade, const std::string& name, int price, int atk, int def, int hp, int dodgeRate, int criticalRate) : Item(grade, ItemType::소비, name, price), atk(atk), def(def), hp(hp), dodgeRate(dodgeRate), criticalRate(criticalRate) {}

void ConsumptionItem::getStatus(int& itemAtk, int& itemDef, int& itemHp, int& itemDR, int& itemCR) const
{
	itemAtk = atk;
	itemDef = def;
	itemHp = hp;
	itemDR = dodgeRate;
	itemCR = criticalRate;
}

void ConsumptionItem::printInfo()
{
	switch (grade)
	{
	case Grade::COMMON:
		setcolor(10,0);
		cout << "아이템명 : " << name << endl;
		setcolor(15,0);
		cout << "가격 : " << price << endl;
		cout << "등급 : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : LEGENDARY" << endl;
		break;
	}
	cout << "{사용 효과}" << endl;
	if(atk != 0)
	cout << "공격력이 " << atk << " 증가합니다." << endl;
	if(def != 0)
	cout << "방어력이 " << def << " 증가합니다." << endl;
	if(hp != 0)
	cout << "체력이 " << hp << " 증가합니다." << endl;
	if(dodgeRate != 0)
	cout << "회피율이 " << dodgeRate << " 증가합니다." << endl;
	if(criticalRate != 0)
	cout << "크리율이 " << criticalRate << " 증가합니다." << endl;
}
#pragma endregion

#pragma region EquipItem
EquipItem::EquipItem(Grade grade, const std::string& name, int price, EquipType equipType, int atk, int def, int hp, int dodgeRate, int criticalRate)
	:Item(grade, ItemType::장비, name, price), equipType(equipType), atk(atk), def(def), hp(hp), dodgeRate(dodgeRate), criticalRate(criticalRate) {}

void EquipItem::printInfo()
{
	switch (grade)
	{
	case Grade::COMMON:
		setcolor(10, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : LEGENDARY" << endl;
		break;
	}
	switch (equipType)
	{
	case EquipType::투구:
		cout << "종류 : 투구" << endl;
		break;
	case EquipType::상의:
		cout << "종류 : 상의" << endl;
		break;
	case EquipType::하의:
		cout << "종류 : 하의" << endl;
		break;
	case EquipType::장갑:
		cout << "종류 : 장갑" << endl;
		break;
	case EquipType::신발:
		cout << "종류 : 신발" << endl;
		break;
	case EquipType::무기:
		cout << "종류 : 무기" << endl;
		break;
	default:
		break;
	}
	if (atk != 0) cout << "공격력 : " << atk << endl;
	if (def != 0) cout << "방어력 : " << def << endl;
	if (hp != 0) cout << "체력 : " << hp << endl;
	if (dodgeRate != 0) cout << "회피율 : " << dodgeRate << '%' << endl;
	if (criticalRate != 0) cout << "크리율 : " << criticalRate << '%' << endl;
}

void EquipItem::getStatus(int& itemAtk, int& itemDef, int& itemHp, int& itemDR, int& itemCR) const
{
	itemAtk = atk;
	itemDef = def;
	itemHp = hp;
	itemDR = dodgeRate;
	itemCR = criticalRate;
}

EquipType EquipItem::getEquipType() const
{
	return equipType;
}

ItemType Item::getItemType() const
{
	return type;
}

std::string Item::getName() const
{
	return name;
}

int Item::getPrice() const
{
	return price;
}

void Item::setPrice(int price)
{
	this->price = price;
}
#pragma endregion

#pragma region OtherItem

#pragma endregion

OtherItem::OtherItem(Grade grade, const std::string& name, int price) : Item(grade, ItemType::기타, name, price) {}

void OtherItem::printInfo()
{
	switch (grade)
	{
	case Grade::COMMON:
		setcolor(10, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "아이템명 : " << name << endl;
		setcolor(15, 0);
		cout << "가격 : " << price << endl;
		cout << "등급 : LEGENDARY" << endl;
		break;
	}
}
