#include "TRPG_Game.h"

#define SERVERIP ""
#define SERVERPORT 54000

using std::cin;
using std::cout;
using std::endl;

void TRPG_Game::connectServer()
{
	cout << "���� ������..." << endl;
	client->Connect();
}

void TRPG_Game::loadItemDB()
{
	cout << "������ ������ �о���� ���Դϴ�..." << endl;
	client->Send("LoadItem");

	int count = stoi(client->Receive());

	for (int i = 0; i < count; i++)
	{
		const int id = stoi(client->Receive());
		const std::string name = client->Receive();
		std::string grade = client->Receive();
		int price = stoi(client->Receive());
		std::string type = client->Receive();
		std::string equipType = client->Receive();
		int atk = stoi(client->Receive());
		int def = stoi(client->Receive());
		int hp = stoi(client->Receive());
		int dodgeRate = stoi(client->Receive());
		int criticalRate = stoi(client->Receive());


		Grade itemGrade;
		if (grade == "COMMON")itemGrade = Grade::COMMON;
		else if (grade == "RARE")itemGrade = Grade::RARE;
		else if (grade == "EPIC")itemGrade = Grade::EPIC;
		else if (grade == "LEGENDARY")itemGrade = Grade::LEGENDARY;
		else itemGrade = Grade::NONE;

		EquipType itemEquipType;
		if (equipType == "HELMET") itemEquipType = EquipType::����;
		else if (equipType == "TOP") itemEquipType = EquipType::����;
		else if (equipType == "BOTTOMS") itemEquipType = EquipType::����;
		else if (equipType == "GLOVES") itemEquipType = EquipType::�尩;
		else if (equipType == "SHOES") itemEquipType = EquipType::�Ź�;
		else if (equipType == "WEAPON") itemEquipType = EquipType::����;
		else itemEquipType = EquipType::NOTEQUIP;

		if (type == "EQUIP")
		{
			Item* item = new EquipItem(itemGrade, name, price, itemEquipType, atk, def, hp, dodgeRate, criticalRate);
			itemdb.addItem(id, item);
		}
		else if (type == "CONSUMPTION")
		{
			Item* item = new ConsumptionItem(itemGrade, name, price, atk, def, hp, dodgeRate, criticalRate);
			itemdb.addItem(id,item);
		}
		else if (type == "OTHER")
		{
			Item* item = new OtherItem(itemGrade, name, price);
			itemdb.addItem(id, item);
		}
		else
		{
			cout << "Item type Error" << endl;
		}
	}
	cout << "������ ���� �ε� �Ϸ�" << endl;
}

void TRPG_Game::printStartScreen()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n";
	printCentered("[Player VS Monster]\n");
	printCentered("ver. 1.0\n\n");
	cout << endl;
	blinkText(paddingCenter("Press Any Button To Continue"),500);
	system("cls");
}

void TRPG_Game::printRules()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	system("cls");
	cout << "\n\n\n\n\n\n\n";
	printCentered("[���� ��Ģ]\n\n");
	printCentered("�� �÷��̾� 1���� �����ϴ� �����Դϴ�.\n\n");
	printCentered("�� ���� ������ 4���� �����Ǿ� �ֽ��ϴ�.\n\n");
	printCentered("�� ��κ��� �Է��� Ű������ [����Ű]�� ���� ó���մϴ�.\n\n");
	printCentered("�� ���װ� �����մϴ�. ���� ��Ź�մϴ�.\n\n");
	cout << endl;
	blinkText(paddingCenter("Press Any Button To Continue"), 500);
	system("cls");
}

void TRPG_Game::signScreen()
{
	int sel;
	
	while(true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		cout << "\n\n\n\n\n\n\n\n\n\n\n";
		printCentered("[Player VS Monster]\n\n");
		printCentered("1. �α���\n\n");
		printCentered("2. ȸ������\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			signIn();
			return;

		case 2:
			system("cls");
			signUp();
			return;
		default:
			system("cls");
			printCentered("�߸��� �Է��Դϴ�!\n");
			break;
		}
	}

}

void TRPG_Game::signIn()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	cursorView(true);

	std::string id, pw;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
	printCentered("[�α���]\n" );
	cout << '\n';
	printCentered("���̵� :        ");
	cout << "\b\b\b\b\b\b\b";
	getline(std::cin, id);
	cout << '\n';
	printCentered("��й�ȣ :      ");
	cout << "\b\b\b\b\b";
	while (true)
	{
		char ch = _getch();

		if (ch == 13)
		{
			break;
		}
		else if (ch == 8)
		{
			if (!pw.empty())
			{
				pw.pop_back();
				std::cout << "\b \b";
			}
		}
		else
		{
			pw.push_back(ch);
			std::cout << '*';
		}
	}
	client->Send("SignIn");
	client->Send(id);
	client->Send(pw);

	std::string ans = client->Receive();
	system("cls");
	if (ans == "Success")
	{
		cout << "�α��� ����" << endl;
		cout << "ĳ���� ���� �ε���..." << endl;
		userId = id;
		std::string job = client->Receive();
		int level = stoi(client->Receive());
		int exp = stoi(client->Receive());
		int atk = stoi(client->Receive());
		int def = stoi(client->Receive());
		int hp = stoi(client->Receive());
		int dodgeRate = stoi(client->Receive());
		int criticalRate = stoi(client->Receive());
		int money = stoi(client->Receive());

		if (job == "HUMAN") player = new Human(&itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money);
		else if (job == "ORC") player = new Orc(&itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money);
		else if (job == "ELF") player = new Elf(&itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money);
		else if (job == "DRACULA") player = new Dracula(&itemdb, level, exp, atk, def, hp, dodgeRate, criticalRate, money);
		loadPlayerInven();
		loadShop();
	}
	else if (ans == "Fail")
	{
		printCentered("�߸��� ��й�ȣ �Դϴ�.\n");
		signIn();
	}
	else if (ans == "NoID")
	{
		printCentered("���Ե��� ���� ID�Դϴ�.\n");
		signIn();
	}
	cursorView(false);
}

void TRPG_Game::signUp()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	cursorView(true);

	std::string id, pw, pwCheck;
	cout << "\n\n\n\n\n\n\n\n\n\n\n";
	printCentered("[ȸ������]\n");
	cout << '\n';
	printCentered("���̵� :        ");
	cout << "\b\b\b\b\b\b\b";
	getline(std::cin, id);

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	cout << '\n';
	printCentered("��й�ȣ :      ");
	cout << "\b\b\b\b\b";
	while (true)
	{
		char ch = _getch();

		if (ch == 13)
		{
			break;
		}
		else if (ch == 8)
		{
			if (!pw.empty())
			{
				pw.pop_back();
				std::cout << "\b \b";
			}
		}
		else
		{
			pw.push_back(ch);
			std::cout << '*';
		}
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	cout << "\n\n";
	printCentered("��й�ȣȮ�� : ");
	while (true) {
		char ch = _getch();

		if (ch == 13) {
			break;
		}
		else if (ch == 8) {
			if (!pwCheck.empty()) {
				pwCheck.pop_back();
				std::cout << "\b \b";
			}
		}
		else {
			pwCheck.push_back(ch);
			std::cout << '*';
		}
	}

	if (pw != pwCheck)
	{
		system("cls");
		printCentered("��й�ȣ�� ��й�ȣȮ���� �ٸ��ϴ�.\n");
		signUp();
	}
	else if (!newIDCheck(id))
	{
		system("cls");
		printCentered("�̹� ������� ���̵��Դϴ�.\n");
		signUp();
	}
	else
	{
		userId = id;
		cursorView(false);
		selClass(pw);
	}
}

bool TRPG_Game::newIDCheck(std::string& id)
{
	client->Send("NewIDCheck");
	client->Send(id);

	if (client->Receive() == "New")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TRPG_Game::selClass(std::string& pw)
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	int sel;

	system("cls");
	while (true)
	{
		cout << "\n\n";
		printCentered("Player�� ������ �����ϼ���.\n\n");
		printCentered("[���� ���]\n\n");
		printCentered("1. �ΰ�\n");
		printCentered("- �ΰ��� ������ �����ϴ� �����Դϴ�.\n");
		printCentered("- �⺻ �ɷ�ġ�� ������, ���ݷ°� ������ ���� �ø� �� �ֽ��ϴ�.\n");
		printCentered("- ���� �ڰ� ȸ���� �����ϸ� �Ϲ� ������ �����մϴ�.\n\n");
		printCentered("2. ��ũ\n");
		printCentered("- ��ũ�� ���� �ܼ������� ������ �����Դϴ�.\n");
		printCentered("- �⺻ �ɷ�ġ�� ���� ��� �Ϲ� ���ݸ� �����մϴ�.\n\n");
		printCentered("3. ����\n");
		printCentered("- ������ ��ø�ϰ� ġ������ ������ �ϴ� �����Դϴ�.\n");
		printCentered("- ���� ġ��Ÿ�� ȸ������ ���� �ø� �� �ֽ��ϴ�.\n");
		printCentered("- ���ط��� 50%�� ���� ���ط� ���ϴ�.\n\n");
		printCentered("4. ���ŧ��\n");
		printCentered("- ���ŧ��� ü���� ������� �� ��ų�� ����ϴ� �����Դϴ�.\n");
		printCentered("- ������ ���ظ� �ָ� ü���� ȸ���ϰų�, ü���� �Ҹ��Ͽ� ������ ������ �����մϴ�.\n\n");
		cout << "" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			player = new Human(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player�� �ΰ� �����Դϴ�.\n");
			serverSignUp(pw,"HUMAN");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 2:
			system("cls");
			player = new Orc(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player�� ��ũ �����Դϴ�.\n");
			serverSignUp(pw, "ORC");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 3:
			system("cls");
			player = new Elf(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player�� ���� �����Դϴ�.\n");
			serverSignUp(pw, "ELF");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 4:
			system("cls");
			player = new Dracula(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player�� ���ŧ�� �����Դϴ�.\n");
			serverSignUp(pw, "DRACULA");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		default:
			system("cls");
			printCentered("�߸��� �Է��Դϴ�!\n");
			break;
		}
	}
}

void TRPG_Game::serverSignUp(std::string& pw, const std::string& job)
{
	client->Send("SignUp");
	client->Send(userId);
	client->Send(pw);
	client->Send(job);
	int atk, def, hp, dodgeRate, criticalRate;
	player->getStatus(atk, def, hp, dodgeRate, criticalRate);
	client->Send(std::to_string(atk));
	client->Send(std::to_string(def));
	client->Send(std::to_string(hp));
	client->Send(std::to_string(dodgeRate));
	client->Send(std::to_string(criticalRate));
}

void TRPG_Game::loadShop()
{
	printCentered("���� ������ �ҷ����� ���Դϴ�...\n");
	client->Send("LoadShop");
	client->Send(userId);

	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int amount = stoi(client->Receive());

		shop->addItem(ShopItemStruct(item_id, amount));
	}
	printCentered("���� �ε� �Ϸ�\n");
}

void TRPG_Game::shopInit()
{
	printCentered("������ ��ǰ�� �غ��ϴ� ���Դϴ�...\n");
	client->Send("ShopInit");
	client->Send(userId);

	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int amount = stoi(client->Receive());

		shop->addItem(ShopItemStruct(item_id, amount));
	}
	printCentered("��ǰ �غ� �Ϸ�\n");
}

void TRPG_Game::loadPlayerInven()
{
	printCentered("�κ��丮 ������ �ҷ����� ���Դϴ�...\n");
	client->Send("LoadInventory");
	client->Send(userId);

	int count = stoi(client->Receive());
	for (int i=0;i<count;i++)
	{
		int item_id = stoi(client->Receive());
		int amount = stoi(client->Receive());
		std::string equipped = client->Receive();
		bool equippedBool = NULL;
		if(equipped != "NULL")
		{
			equippedBool = stoi(equipped);
		}
		player->addItem(ItemStruct(item_id, amount, equippedBool));
	}
	printCentered("�κ��丮 �ε� �Ϸ�\n");
}

void TRPG_Game::loadTradeList(const std::string& type)
{
	printCentered("��ϵ� �ŷ� ����� �ҷ����� ���Դϴ�...\n");
	client->Send("GetTradeList");
	client->Send(type);

	tradeShop->clearList();
	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int price = stoi(client->Receive());

		tradeShop->addItem(TradeShopItemStruct(item_id, price));
	}
	printCentered("�ŷ� ��� �ε� �Ϸ�\n");
}

void TRPG_Game::loadMyTradeList(std::string userId)
{
	printCentered("��ϵ� �ŷ� ����� �ҷ����� ���Դϴ�...\n");
	client->Send("GetMyTradeList");
	client->Send(userId);

	tradeShop->clearList();
	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int price = stoi(client->Receive());

		tradeShop->addItem(TradeShopItemStruct(item_id, price));
	}
	printCentered("�ŷ� ��� �ε� �Ϸ�\n");
}

void TRPG_Game::mainMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n" << endl;
		printCentered("[���� �޴�]\n\n");
		printCentered("1. ����\n");
		printCentered("2. ����\n");
		printCentered("3. �ŷ���\n");
		printCentered("4. �κ��丮\n");
		printCentered("5. �������ͽ� Ȯ��\n");
		printCentered("6. ��ŷ Ȯ��\n");
		printCentered("7. ��������\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			fight();
			continue;

		case 2:
			shopMenu();
			continue;

		case 3:
			tradeMenu();
			continue;

		case 4:
			inventoryMenu();
			continue;

		case 5:
			system("cls");
			player->printStatus();
			blinkText("�ڷ� ���ư����� �ƹ� Ű�� ��������", 500);
			continue;

		case 6:
			rankCheck();
			continue;

		case 7:
			system("cls");
			cout << "\n\n\n\n\n\n\n\n\n\n\n";
			printCentered("������ �����մϴ�...\n");
			Sleep(3000);
			exit(0);

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::shopMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("[���� �޴�]\n\n");
		printCentered("1. ����\n");
		printCentered("2. �Ǹ�\n");
		printCentered("3. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			shopMenu_buyItemMenu();
			continue;

		case 2:
			system("cls");
			shopMenu_sellItemMenu();
			continue;

		case 3:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::shopMenu_buyItemMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�����ϱ�]\n\n");
		printCentered("1. ���\n");
		printCentered("2. �Һ�\n");
		printCentered("3. ��Ÿ\n");
		printCentered("4. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			shopMenu_buyItem(ItemType::���);
			break;

		case 2:
			system("cls");
			shopMenu_buyItem(ItemType::�Һ�);
			break;

		case 3:
			system("cls");
			shopMenu_buyItem(ItemType::��Ÿ);
			break;

		case 4:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::shopMenu_buyItem(ItemType type)
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		shop->showList(type);
		cout << "���� ������ : " << player->getMoney() << endl;
		cout << "�����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > shop->getListSize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			if (player->getMoney() < shop->getItemPrice(type,sel))
			{
				cout << "�������� �����մϴ�." << endl;
			}
			else
			{
				int itemId = shop->sellItem(type,sel);
				player->buy(itemId);

				client->Send("Transaction");
				client->Send(userId);
				client->Send(std::to_string(itemId));
				client->Send("BUY");
				client->Send(std::to_string(itemdb.getItem(itemId)->getPrice()));
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::shopMenu_sellItemMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�Ǹ��ϱ�]\n\n");
		printCentered("1. ���\n");
		printCentered("2. �Һ�\n");
		printCentered("3. ��Ÿ\n");
		printCentered("4. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			shopMenu_sellItem(ItemType::���);
			continue;

		case 2:
			system("cls");
			shopMenu_sellItem(ItemType::�Һ�);
			continue;

		case 3:
			system("cls");
			shopMenu_sellItem(ItemType::��Ÿ);
			continue;

		case 4:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::shopMenu_sellItem(ItemType type)
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		player->printInventory(type);
		cout << "�Ǹ��Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			if (type == ItemType::��� && player->getEquipped(sel))
			{
				cout << "�������� �������Դϴ�." << endl;
			}
			else
			{
				int itemId = player->sellItem(type,sel);
				shop->buy(itemId);

				client->Send("Transaction");
				client->Send(userId);
				client->Send(std::to_string(itemId));
				client->Send("SELL");
				client->Send(std::to_string(itemdb.getItem(itemId)->getPrice()));
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::inventoryMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�κ��丮 �޴�]\n\n");
		printCentered("1. �κ��丮 Ȯ��\n");
		printCentered("2. �����ϱ�\n");
		printCentered("3. �����ϱ�\n");
		printCentered("4. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			inventoryMenu_showItem();
			continue;

		case 2:
			system("cls");
			inventoryMenu_equipItem();
			continue;

		case 3:
			system("cls");
			inventoryMenu_unequipItem();
			continue;

		case 4:
			return;

		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::inventoryMenu_showItem()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	int sel = 1;
	while (true)
	{
		system("cls");
		switch (sel)
		{
		case 1:
			player->printInventory(ItemType::���);
			break;

		case 2:
			player->printInventory(ItemType::�Һ�);
			break;

		case 3:
			player->printInventory(ItemType::��Ÿ);
			break;

		case 4:
			return;

		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}
		cout << "������ : " << player->getMoney() << endl;
		cout << "1 : ���\t2 : �Һ�\t3 : ��Ÿ\t4 : �ڷΰ���" << endl;
		sel = _getch() - '0';
	}
}

void TRPG_Game::inventoryMenu_equipItem()
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		player->printInventory(ItemType::���);
		cout << "�����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::���) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			EquipItem* item = dynamic_cast<EquipItem*>(itemdb.getItem(player->getItem(ItemType::���,sel)));


			if (player->getEquipped(sel))
			{
				cout << item->getName() << "�� �̹� �������Դϴ�." << endl;
			}
			else if (!player->equippedCheck(item))
			{
				player->setEquipped(sel, true);
				cout << item->getName() << "�� �����߽��ϴ�." << endl;
				player->equip(item);

				client->Send("EquipItem");
				client->Send(userId);
				client->Send(std::to_string(player->getItem(ItemType::���,sel)));
				client->Send("1");
			}
			else
			{
				std::string type = "ERROR";
				switch (item->getEquipType())
				{
				case EquipType::����:
					type = "����";
					break;
				case EquipType::����:
					type = "����";
					break;
				case EquipType::����:
					type = "����";
					break;
				case EquipType::�尩:
					type = "�尩";
					break;
				case EquipType::�Ź�:
					type = "�Ź�";
					break;
				case EquipType::����:
					type = "����";
					break;
				}
				cout << type << "��(��) �̹� �������Դϴ�." << endl;
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::inventoryMenu_unequipItem()
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		player->printInventory(ItemType::���);
		cout << "���� �����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::���) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			EquipItem* item = dynamic_cast<EquipItem*>(itemdb.getItem(player->getItem(ItemType::���,sel)));

			if (player->getEquipped(sel))
			{
				player->setEquipped(sel,false);
				cout << item->getName() << "�� �������� �߽��ϴ�." << endl;
				player->unequip(item);

				client->Send("EquipItem");
				client->Send(userId);
				client->Send(std::to_string(player->getItem(ItemType::���,sel)));
				client->Send("0");
			}
			else
			{
				cout << item->getName() << "�� �������� �ƴմϴ�." << endl;
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::fight()
{
	int pAtk, pDef, pHp, pDR, pCR;
	player->getStatus(pAtk, pDef, pHp, pDR, pCR);

	std::string location;

	chooseLocation(location);

	if (location == "MAINMENU") return;

	while (true)
	{
		system("cls");
		playerTurn();
		Sleep(2000);
		if (monsterDead())
		{
			player->setStatus(pAtk, pDef, pHp, pDR, pCR);
			int rewardMoney, rewardExp, rewardItem;
			monster->reward(rewardMoney, rewardExp, rewardItem);
			player->gainReward(rewardMoney, rewardExp, rewardItem);
			int levelUpCount = player->levelUp();
			if (levelUpCount != 0)
			{
				client->Send("LevelUp");
				client->Send(userId);
				client->Send(std::to_string(player->getLevel()));
				client->Send(std::to_string(levelUpCount));
			}

			client->Send("BattleRecord");
			client->Send(userId);
			client->Send(std::to_string(monster->getID()));
			client->Send("1");
			break;
		}
		system("cls");
		monsterTurn();
		Sleep(2000);
		if (playerDead())
		{
			player->setStatus(pAtk, pDef, pHp, pDR, pCR);
			player->loseExp();
			client->Send("BattleRecord");
			client->Send(userId);
			client->Send(std::to_string(monster->getID()));
			client->Send("0");
			break;
		}
	}
	cout << "���� �޴��� ���ư��ϴ�..." << endl;
	Sleep(1500);
}

void TRPG_Game::chooseLocation(std::string& location)
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("1. �ʺ��� �����\n");
		printCentered("2. �߱��� �����\n");
		printCentered("3. ����� �����\n");
		printCentered("4. ���� �޴�\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			location = "EASY";
			generateEnemy(location);
			return;

		case 2:
			location = "NORMAL";
			generateEnemy(location);
			return;

		case 3:
			location = "HARD";
			generateEnemy(location);
			return;

		case 4:
			location = "MAINMENU";
			return;

		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::generateEnemy(const std::string& location)
{
	client->Send("GenerateEnemy");
	client->Send(location);

	int id = stoi(client->Receive());
	std::string name = client->Receive();
	int atk = stoi(client->Receive());
	int def = stoi(client->Receive());
	int hp = stoi(client->Receive());
	int ciriticalRate = stoi(client->Receive());
	int dodgeRate = stoi(client->Receive());
	int rewardGold = stoi(client->Receive());
	int rewardExp = stoi(client->Receive());
	std::string str = client->Receive();
	int rewardItemID;
	if (str != "") rewardItemID = stoi(str);
	else rewardItemID = NULL;

	if (location == "EASY")
	{
		monster = new EasyMonster(id, name, atk, def, ciriticalRate, dodgeRate, hp, rewardGold, rewardExp, rewardItemID);
	}
	else if (location == "NORMAL")
	{
		monster = new NormalMonster(id, name, atk, def, ciriticalRate, dodgeRate, hp, rewardGold, rewardExp, rewardItemID);
	}
	else//hard
	{
		monster = new HardMonster(id, name, atk, def, ciriticalRate, dodgeRate, hp, rewardGold, rewardExp, rewardItemID);
	}
}

void TRPG_Game::playerTurn()
{
	int atkdamage = 0, pureDamage = 0;
	bool notActed = true;

	while (notActed)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "[player�� ��]\n" << endl;

		cout << "[�� ����]" << endl;
		monster->printInfo();
		cout << endl;

		cout << "[���� ����]" << endl;
		player->printStatus();
		cout << endl;

		int sel;

		cout << "1. �ൿ�ϱ�" << endl;
		cout << "2. �Һ������ ����ϱ�" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			cout << "[player�� ��]\n" << endl;

			cout << "[�� ����]" << endl;
			monster->printInfo();
			cout << endl;

			cout << "[���� ����]" << endl;
			player->printStatus();
			cout << endl;
			cout << "\n";
			player->Act(atkdamage, pureDamage);
			notActed = false;
			break;

		case 2:
			cout << "\n";
			useConsumptionItem();
			break;

		default:
			system("cls");
			cout << "�߸��� �Է��Դϴ�." << endl;
			break;
		}

		
	}

	monster->TakeDamage(atkdamage, pureDamage);
}

void TRPG_Game::useConsumptionItem()
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		player->printInventory(ItemType::�Һ�);
		cout << "����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::�Һ�) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			break;
		}
		else
		{
			bool usedAll = false;
			int usedItemID = player->useConsumptionItem(sel, usedAll);

			client->Send("UseItem");
			client->Send(userId);
			client->Send(std::to_string(usedItemID));
			client->Send(std::to_string(usedAll));

			Sleep(2000);
		}
	}
	cursorView(false);
}

bool TRPG_Game::playerDead()
{
	if (player->getHealth() <= 0)
	{
		system("cls");
		cout << "player ���!" << endl;
		return true;
	}
	return false;
}

void TRPG_Game::tradeMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�ŷ��� �޴�]\n\n");
		printCentered("1. ����\n");
		printCentered("2. ���\n");
		printCentered("3. ��� ���\n");
		printCentered("4. ��� ����\n");
		printCentered("5. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			tradeMenu_buyItemMenu();
			break;

		case 2:
			system("cls");
			tradeMenu_sellItemMenu();
			break;

		case 3:
			system("cls");
			tradeMenu_cancelTrade();
			break;

		case 4:
			system("cls");
			tradeMenu_receivePayment();
			break;

		case 5:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::tradeMenu_buyItemMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�ŷ��ϱ�]\n\n");
		printCentered("1. ���\n");
		printCentered("2. �Һ�\n");
		printCentered("3. ��Ÿ\n");
		printCentered("4. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			loadTradeList("EQUIP");
			tradeMenu_buyItem();
			break;

		case 2:
			system("cls");
			loadTradeList("CONSUMPTION");
			tradeMenu_buyItem();
			break;

		case 3:
			system("cls");
			loadTradeList("OTHER");
			tradeMenu_buyItem();
			break;

		case 4:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::tradeMenu_buyItem()
{
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		tradeShop->showList();
		cout << "���� ������ : " << player->getMoney() << endl;
		cout << "�����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > tradeShop->getListSize() || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			int price = tradeShop->getItemPrice(sel);
			int itemId = tradeShop->getItemId(sel);
			if (player->getMoney() < price)
			{
				cout << "�������� �����մϴ�." << endl;
			}
			else
			{
				client->Send("TradeBuy");
				client->Send(userId);
				client->Send(std::to_string(itemId));
				client->Send(std::to_string(price));

				std::string result = client->Receive();
				if (result == "Success")
				{
					tradeShop->sellItem(sel);
					player->tradeBuyItem(itemId,price);
					cout << itemdb.getItem(itemId)->getName() << "���ԿϷ�" << endl;
					cout << "���� ������ : " << player->getMoney() << endl;
				}
				else
				{
					cout << "�̹� �Ǹŵ� ��ǰ�Դϴ�." << endl;
				}
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::tradeMenu_sellItemMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n\n" << endl;
		printCentered("[�ŷ� ��� �ϱ�]\n\n");
		printCentered("1. ���\n");
		printCentered("2. �Һ�\n");
		printCentered("3. ��Ÿ\n");
		printCentered("4. �ڷΰ���\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			tradeMenu_sellItem(ItemType::���);
			break;

		case 2:
			system("cls");
			tradeMenu_sellItem(ItemType::�Һ�);
			break;

		case 3:
			system("cls");
			tradeMenu_sellItem(ItemType::��Ÿ);
			break;

		case 4:
			return;

		default:
			printCentered("�߸��� �Է��Դϴ�.\n");
			Sleep(1000);
			break;
		}
	}
}

void TRPG_Game::tradeMenu_sellItem(ItemType type)
{
	cursorView(true);
	int sel, price;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		player->printInventory(type);
		cout << "�Ǹ��Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			if (type == ItemType::��� && player->getEquipped(sel))
			{
				cout << "�������� �������Դϴ�." << endl;
			}
			else
			{
				cout << "�Ǹ��Ͻ� �������� ������ �Է��ϼ��� : ";
				cin >> price;
				bool soldAll;
				int itemId = player->tradeSellItem(type, sel, soldAll);

				client->Send("TradeSell");
				client->Send(userId);
				client->Send(std::to_string(itemId));
				client->Send(std::to_string(price));
				client->Send(std::to_string(soldAll));
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::tradeMenu_cancelTrade()
{
	loadMyTradeList(userId);
	cursorView(true);
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		tradeShop->showList();
		cout << "���� ������ : " << player->getMoney() << endl;
		cout << "�ŷ� ����� ����Ͻ� �������� ��ȣ�� �Է��ϼ���(0 �Է½� �ڷΰ���) : ";

		std::cin >> sel;

		if (cin.fail() || sel > tradeShop->getListSize() || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "�߸��� �Է��Դϴ�." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			int price = tradeShop->getItemPrice(sel);
			int itemId = tradeShop->getItemId(sel);

			client->Send("CancelTrade");
			client->Send(userId);
			client->Send(std::to_string(itemId));
			client->Send(std::to_string(price));

			std::string result = client->Receive();
			if (result == "Success")
			{
				tradeShop->sellItem(sel);
				player->tradeBuyItem(itemId, -price);
				cout << itemdb.getItem(itemId)->getName() << "�ŷ� ��� ��� �Ϸ�" << endl;
				cout << "���� ������ : " << player->getMoney() << endl;
			}
			else
			{
				cout << "�̹� �Ǹŵ� ��ǰ�Դϴ�." << endl;
			}
			Sleep(2000);
		}
	}
	cursorView(false);
}

void TRPG_Game::tradeMenu_receivePayment()
{
	client->Send("ReceivePayment");
	client->Send(userId);

	std::string responce = client->Receive();
	if (responce == "None")
	{
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
		printCentered("������� ����� �����ϴ�.");
		Sleep(1500);
	}
	else
	{
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
		printCentered(responce + "�� ���� ����� �޾ҽ��ϴ�.");
		player->receivePayment(stoi(responce));
		Sleep(1500);
	}
}

void TRPG_Game::rankCheck()
{
	client->Send("RankCheck");
	int count = stoi(client->Receive());
	system("cls");
	cout << "\n\n\n\n";
	printCentered("[����ǥ]\n");
	cout << endl << endl;
	for (int i = 0; i < count; i++)
	{
		std::string id = client->Receive();
		std::string level = client->Receive();
		printCentered("RANK : " + std::to_string(i + 1) + " / LEVEL : " + level + " / ID : " + id);
		cout << endl << endl;
	}

	cout << endl;
	blinkText(paddingCenter("�ڷ� ���ư����� �ƹ� Ű�� ��������"),500);
}

void TRPG_Game::monsterTurn()
{
	int atkdamage = 0, pureDamage = 0;

	system("cls");
	cout << "[Monster�� ��]\n" << endl;

	Sleep(1000);
	monster->Act(atkdamage, pureDamage);
	Sleep(500);
	player->Defence(atkdamage, pureDamage);
}

bool TRPG_Game::monsterDead()
{
	if (monster->getHealth() <= 0)
	{
		system("cls");
		cout << "�� óġ ����!" << endl;
		return true;
	}
	return false;
}

TRPG_Game::TRPG_Game()
{
	client = new Client(SERVERIP, SERVERPORT);
	shop = new Shop(&itemdb);
	tradeShop = new TradeShop(&itemdb);
	player = nullptr;
	monster = nullptr;
}

TRPG_Game::~TRPG_Game()
{
	delete player;
	player = nullptr;
	delete monster;
	monster = nullptr;
	delete shop;
	shop = nullptr;
	delete tradeShop;
	tradeShop = nullptr;
	delete client;
	client = nullptr;
}

void TRPG_Game::gameStart()
{
	cursorView(false);
	connectServer();
	loadItemDB();
	printStartScreen();
	printRules();
	signScreen();
	mainMenu();
	cursorView(true);
}