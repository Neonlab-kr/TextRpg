#include "Monster.h"

using std::cout;
using std::endl;

#pragma region Monster
void Monster::printInfo() const
{
	cout << "�̸� : " << name << endl;
	cout << "���ݷ� : " << atk << endl;
	cout << "���� : " << def << endl;
	cout << "ũ��Ƽ�� Ȯ�� : " << critRate << "%" << endl;
	cout << "ȸ�� Ȯ�� : " << dodgeRate << "%" << endl;
	cout << "���� ü�� : " << health << endl;
}

void Monster::TakeDamage(int atkDamage, int pureDamage)
{
	if (atkDamage != 0 || pureDamage != 0)
	{
		if (rateCheck(dodgeRate))
		{
			cout << name <<"�� ȸ�ǿ� �����߽��ϴ�." << endl;
		}
		else
		{
			int damage;
			damage = (atkDamage - def);
			if (damage <= 0) damage = 0;
			damage += pureDamage;
			cout << name << "�� " << damage << "�� ���ظ� �Ծ����ϴ�." << endl;
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
	cout << name << "�� �Ϲ� ����!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 1.5�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ¸�ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk;
		cout << "���� : " << atkdamage << endl;
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
	cout << name << "�� �Ϲ� ����!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 1.5�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ¸�ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk;
		cout << "���� : " << atkdamage << endl;
	}
}

void NormalMonster::strongAttack(int& atkdamage)
{
	cout << name << "�� ���� ����!" << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 3�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 3;
		cout << "���� : " << atkdamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ��� 1.5�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		cout << "���� : " << atkdamage << endl;
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
	cout << name << "�� �Ϲ� ����! ���̵��� ���� ������ ������ ������������ ����˴ϴ�." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 2�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk;
		pureDamage = atk;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ¸�ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk/2;
		pureDamage = atk/2;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
	}
}

void HardMonster::strongAttack(int& atkdamage, int& pureDamage)
{
	cout << name << "�� ���� ����! ���̵��� ���� ������ ������ ������������ ����˴ϴ�." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 3�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 1.5;
		pureDamage = atk * 1.5;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ��� 2�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk;
		pureDamage = atk;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
	}
}

void HardMonster::specialAttack(int& atkdamage, int& pureDamage)
{
	cout << name << "�� Ư�� ����! ���̵��� ���� ������ ������ ������������ ����˴ϴ�." << endl;
	Sleep(500);
	bool crit = rateCheck(critRate);
	if (crit)
	{
		cout << "ġ��Ÿ ����!" << name << "�� ���ݷ��� 10�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 5;
		pureDamage = atk * 5;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
	}
	else
	{
		cout << name << "�� ���ݷ��� 5�踸ŭ�� �������� �����մϴ�." << endl;
		Sleep(500);
		atkdamage = atk * 2.5;
		pureDamage = atk * 2.5;
		cout << "���� : " << atkdamage << endl;
		cout << "���������� : " << pureDamage << endl;
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