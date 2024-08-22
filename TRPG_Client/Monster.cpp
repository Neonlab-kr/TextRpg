#include "Monster.h"

using std::cout;
using std::endl;

#pragma region Monster
void Monster::printInfo() const
{
	cout << "이름 : " << name << endl;
	cout << "공격력 : " << atk << endl;
	cout << "방어력 : " << def << endl;
	cout << "크리티컬 확률 : " << critRate << "%" << endl;
	cout << "회피 확률 : " << dodgeRate << "%" << endl;
	cout << "현재 체력 : " << health << endl;
}

void Monster::TakeDamage(int atkDamage, int pureDamage)
{
	if (atkDamage != 0 || pureDamage != 0)
	{
		if (rateCheck(dodgeRate))
		{
			cout << name <<"이 회피에 성공했습니다." << endl;
		}
		else
		{
			int damage;
			damage = (atkDamage - def);
			if (damage <= 0) damage = 0;
			damage += pureDamage;
			cout << name << "이 " << damage << "의 피해를 입었습니다." << endl;
			health -= damage;
		}
	}
}

int Monster::getID() const
{
	return id;
}

int Monster::getHealth() const
{
	return health;
}

void Monster::reward(int& money, int& exp, int& item) const
{
	money = rewardMoney;
	exp = rewardExp;
	item = rewardItemID;
}
#pragma endregion

#pragma region EasyMonster
void EasyMonster::normalAttack(int& atkdamage)
{
	cout << name << "의 일반 공격!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 1.5배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << name << "이 공격력만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk;
		cout << "위력 : " << atkdamage << endl;
	}
}

void EasyMonster::Act(int& atkdamage, int& pureDamage)
{
	normalAttack(atkdamage);
}
#pragma endregion

#pragma region NormalMonster
void NormalMonster::normalAttack(int& atkdamage)
{
	cout << name << "의 일반 공격!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 1.5배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << name << "이 공격력만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk;
		cout << "위력 : " << atkdamage << endl;
	}
}

void NormalMonster::strongAttack(int& atkdamage)
{
	cout << name << "의 강한 공격!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 3배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 3;
		cout << "위력 : " << atkdamage << endl;
	}
	else
	{
		cout << name << "이 공격력의 1.5배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "위력 : " << atkdamage << endl;
	}
}
void NormalMonster::Act(int& atkdamage, int& pureDamage)
{
	srand(time(NULL));
	int sel = rand() % 100 + 1;
	if(sel <70) normalAttack(atkdamage);
	else strongAttack(atkdamage);
}
#pragma endregion

#pragma region HardMonster
void HardMonster::normalAttack(int& atkdamage, int& pureDamage)
{
	cout << name << "의 일반 공격! 난이도에 따라 위력의 절반이 고정데미지로 변경됩니다." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 2배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk;
		pureDamage = atk;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
	else
	{
		cout << name << "이 공격력만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk/2;
		pureDamage = atk/2;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
}

void HardMonster::strongAttack(int& atkdamage, int& pureDamage)
{
	cout << name << "의 강한 공격! 난이도에 따라 위력의 절반이 고정데미지로 변경됩니다." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 3배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		pureDamage = atk * 1.5;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
	else
	{
		cout << name << "이 공격력의 2배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk;
		pureDamage = atk;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
}

void HardMonster::specialAttack(int& atkdamage, int& pureDamage)
{
	cout << name << "의 특수 공격! 난이도에 따라 위력의 절반이 고정데미지로 변경됩니다." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "치명타 적중!" << name << "이 공격력의 10배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 5;
		pureDamage = atk * 5;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
	else
	{
		cout << name << "이 공격력의 5배만큼의 위력으로 공격합니다." << endl;
		Sleep(500);
		atkdamage = atk * 2.5;
		pureDamage = atk * 2.5;
		cout << "위력 : " << atkdamage << endl;
		cout << "고정데미지 : " << pureDamage << endl;
	}
}

void HardMonster::Act(int& atkdamage, int& pureDamage)
{
	srand(time(NULL));
	int sel = rand() % 100 + 1;

	if (sel < 50) normalAttack(atkdamage, pureDamage);
	else if (sel < 90) strongAttack(atkdamage, pureDamage);
	else specialAttack(atkdamage, pureDamage);
}
#pragma endregion