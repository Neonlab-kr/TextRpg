#include "Item.h"

using std::cout;
using std::endl;

#pragma region ConsumptionItem
ConsumptionItem::ConsumptionItem(Grade grade, const std::string& name, int price, int atk, int def, int hp, int dodgeRate, int criticalRate) : Item(grade, ItemType::�Һ�, name, price), atk(atk), def(def), hp(hp), dodgeRate(dodgeRate), criticalRate(criticalRate) {}

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
		cout << "�����۸� : " << name << endl;
		setcolor(15,0);
		cout << "���� : " << price << endl;
		cout << "��� : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : LEGENDARY" << endl;
		break;
	}
	cout << "{��� ȿ��}" << endl;
	if(atk != 0)
	cout << "���ݷ��� " << atk << " �����մϴ�." << endl;
	if(def != 0)
	cout << "������ " << def << " �����մϴ�." << endl;
	if(hp != 0)
	cout << "ü���� " << hp << " �����մϴ�." << endl;
	if(dodgeRate != 0)
	cout << "ȸ������ " << dodgeRate << " �����մϴ�." << endl;
	if(criticalRate != 0)
	cout << "ũ������ " << criticalRate << " �����մϴ�." << endl;
}
#pragma endregion

#pragma region EquipItem
EquipItem::EquipItem(Grade grade, const std::string& name, int price, EquipType equipType, int atk, int def, int hp, int dodgeRate, int criticalRate)
	:Item(grade, ItemType::���, name, price), equipType(equipType), atk(atk), def(def), hp(hp), dodgeRate(dodgeRate), criticalRate(criticalRate) {}

void EquipItem::printInfo()
{
	switch (grade)
	{
	case Grade::COMMON:
		setcolor(10, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : LEGENDARY" << endl;
		break;
	}
	switch (equipType)
	{
	case EquipType::����:
		cout << "���� : ����" << endl;
		break;
	case EquipType::����:
		cout << "���� : ����" << endl;
		break;
	case EquipType::����:
		cout << "���� : ����" << endl;
		break;
	case EquipType::�尩:
		cout << "���� : �尩" << endl;
		break;
	case EquipType::�Ź�:
		cout << "���� : �Ź�" << endl;
		break;
	case EquipType::����:
		cout << "���� : ����" << endl;
		break;
	default:
		break;
	}
	if (atk != 0) cout << "���ݷ� : " << atk << endl;
	if (def != 0) cout << "���� : " << def << endl;
	if (hp != 0) cout << "ü�� : " << hp << endl;
	if (dodgeRate != 0) cout << "ȸ���� : " << dodgeRate << '%' << endl;
	if (criticalRate != 0) cout << "ũ���� : " << criticalRate << '%' << endl;
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

OtherItem::OtherItem(Grade grade, const std::string& name, int price) : Item(grade, ItemType::��Ÿ, name, price) {}

void OtherItem::printInfo()
{
	switch (grade)
	{
	case Grade::COMMON:
		setcolor(10, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : COMMON" << endl;
		break;
	case Grade::RARE:
		setcolor(11, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : RARE" << endl;
		break;
	case Grade::EPIC:
		setcolor(13, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : EPIC" << endl;
		break;
	case Grade::LEGENDARY:
		setcolor(14, 0);
		cout << "�����۸� : " << name << endl;
		setcolor(15, 0);
		cout << "���� : " << price << endl;
		cout << "��� : LEGENDARY" << endl;
		break;
	}
}
