#include "TRPG_Game.h"

#define SERVERIP ""
#define SERVERPORT 54000

using std::cin;
using std::cout;
using std::endl;

void TRPG_Game::connectServer()
{
	cout << "서버 연결중..." << endl;
	client->Connect();
}

void TRPG_Game::loadItemDB()
{
	cout << "아이템 정보를 읽어오는 중입니다..." << endl;
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
		if (equipType == "HELMET") itemEquipType = EquipType::투구;
		else if (equipType == "TOP") itemEquipType = EquipType::상의;
		else if (equipType == "BOTTOMS") itemEquipType = EquipType::하의;
		else if (equipType == "GLOVES") itemEquipType = EquipType::장갑;
		else if (equipType == "SHOES") itemEquipType = EquipType::신발;
		else if (equipType == "WEAPON") itemEquipType = EquipType::무기;
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
	cout << "아이템 정보 로드 완료" << endl;
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
	printCentered("[게임 규칙]\n\n");
	printCentered("● 플레이어 1명이 진행하는 게임입니다.\n\n");
	printCentered("● 현재 직업은 4종류 구현되어 있습니다.\n\n");
	printCentered("● 대부분의 입력은 키보드의 [숫자키]를 통해 처리합니다.\n\n");
	printCentered("● 버그가 가득합니다. 제보 부탁합니다.\n\n");
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
		printCentered("1. 로그인\n\n");
		printCentered("2. 회원가입\n");

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
			printCentered("잘못된 입력입니다!\n");
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
	printCentered("[로그인]\n" );
	cout << '\n';
	printCentered("아이디 :        ");
	cout << "\b\b\b\b\b\b\b";
	getline(std::cin, id);
	cout << '\n';
	printCentered("비밀번호 :      ");
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
		cout << "로그인 성공" << endl;
		cout << "캐릭터 정보 로드중..." << endl;
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
		printCentered("잘못된 비밀번호 입니다.\n");
		signIn();
	}
	else if (ans == "NoID")
	{
		printCentered("가입되지 않은 ID입니다.\n");
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
	printCentered("[회원가입]\n");
	cout << '\n';
	printCentered("아이디 :        ");
	cout << "\b\b\b\b\b\b\b";
	getline(std::cin, id);

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	cout << '\n';
	printCentered("비밀번호 :      ");
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
	printCentered("비밀번호확인 : ");
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
		printCentered("비밀번호와 비밀번호확인이 다릅니다.\n");
		signUp();
	}
	else if (!newIDCheck(id))
	{
		system("cls");
		printCentered("이미 사용중인 아이디입니다.\n");
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
		printCentered("Player의 종족을 선택하세요.\n\n");
		printCentered("[종족 목록]\n\n");
		printCentered("1. 인간\n");
		printCentered("- 인간은 끝없이 성장하는 종족입니다.\n");
		printCentered("- 기본 능력치는 낮지만, 공격력과 방어력을 매턴 올릴 수 있습니다.\n");
		printCentered("- 또한 자가 회복이 가능하며 일반 공격이 가능합니다.\n\n");
		printCentered("2. 오크\n");
		printCentered("- 오크는 가장 단순하지만 강력한 종족입니다.\n");
		printCentered("- 기본 능력치가 높은 대신 일반 공격만 가능합니다.\n\n");
		printCentered("3. 엘프\n");
		printCentered("- 엘프는 민첩하고 치명적인 공격을 하는 종족입니다.\n");
		printCentered("- 매턴 치명타와 회피율을 매턴 올릴 수 있습니다.\n");
		printCentered("- 피해량의 50%는 고정 피해로 들어갑니다.\n\n");
		printCentered("4. 드라큘라\n");
		printCentered("- 드라큘라는 체력을 기반으로 한 스킬을 사용하는 종족입니다.\n");
		printCentered("- 적에게 피해를 주며 체력을 회복하거나, 체력을 소모하여 강력한 공격이 가능합니다.\n\n");
		cout << "" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			player = new Human(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player는 인간 종족입니다.\n");
			serverSignUp(pw,"HUMAN");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 2:
			system("cls");
			player = new Orc(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player는 오크 종족입니다.\n");
			serverSignUp(pw, "ORC");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 3:
			system("cls");
			player = new Elf(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player는 엘프 종족입니다.\n");
			serverSignUp(pw, "ELF");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		case 4:
			system("cls");
			player = new Dracula(&itemdb);
			cout << "\n\n\n\n\n\n\n\n";
			printCentered("Player는 드라큘라 종족입니다.\n");
			serverSignUp(pw, "DRACULA");
			shopInit();
			Sleep(1500);
			system("cls");
			return;

		default:
			system("cls");
			printCentered("잘못된 입력입니다!\n");
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
	printCentered("상점 정보를 불러오는 중입니다...\n");
	client->Send("LoadShop");
	client->Send(userId);

	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int amount = stoi(client->Receive());

		shop->addItem(ShopItemStruct(item_id, amount));
	}
	printCentered("상점 로드 완료\n");
}

void TRPG_Game::shopInit()
{
	printCentered("상인이 상품을 준비하는 중입니다...\n");
	client->Send("ShopInit");
	client->Send(userId);

	int count = stoi(client->Receive());
	for (int i = 0; i < count; i++)
	{
		int item_id = stoi(client->Receive());
		int amount = stoi(client->Receive());

		shop->addItem(ShopItemStruct(item_id, amount));
	}
	printCentered("상품 준비 완료\n");
}

void TRPG_Game::loadPlayerInven()
{
	printCentered("인벤토리 정보를 불러오는 중입니다...\n");
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
	printCentered("인벤토리 로드 완료\n");
}

void TRPG_Game::loadTradeList(const std::string& type)
{
	printCentered("등록된 거래 목록을 불러오는 중입니다...\n");
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
	printCentered("거래 목록 로드 완료\n");
}

void TRPG_Game::loadMyTradeList(std::string userId)
{
	printCentered("등록된 거래 목록을 불러오는 중입니다...\n");
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
	printCentered("거래 목록 로드 완료\n");
}

void TRPG_Game::mainMenu()
{
	int sel;
	while (true)
	{
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		system("cls");
		cout << "\n\n\n\n\n\n\n" << endl;
		printCentered("[메인 메뉴]\n\n");
		printCentered("1. 전투\n");
		printCentered("2. 상점\n");
		printCentered("3. 거래소\n");
		printCentered("4. 인벤토리\n");
		printCentered("5. 스테이터스 확인\n");
		printCentered("6. 랭킹 확인\n");
		printCentered("7. 게임종료\n");

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
			blinkText("뒤로 돌아가려면 아무 키나 누르세요", 500);
			continue;

		case 6:
			rankCheck();
			continue;

		case 7:
			system("cls");
			cout << "\n\n\n\n\n\n\n\n\n\n\n";
			printCentered("게임을 종료합니다...\n");
			Sleep(3000);
			exit(0);

		default:
			printCentered("잘못된 입력입니다.\n");
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
		printCentered("[상점 메뉴]\n\n");
		printCentered("1. 구입\n");
		printCentered("2. 판매\n");
		printCentered("3. 뒤로가기\n");

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
			printCentered("잘못된 입력입니다.\n");
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
		printCentered("[구매하기]\n\n");
		printCentered("1. 장비\n");
		printCentered("2. 소비\n");
		printCentered("3. 기타\n");
		printCentered("4. 뒤로가기\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			shopMenu_buyItem(ItemType::장비);
			break;

		case 2:
			system("cls");
			shopMenu_buyItem(ItemType::소비);
			break;

		case 3:
			system("cls");
			shopMenu_buyItem(ItemType::기타);
			break;

		case 4:
			return;

		default:
			printCentered("잘못된 입력입니다.\n");
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
		cout << "현재 소지금 : " << player->getMoney() << endl;
		cout << "구입하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > shop->getListSize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
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
				cout << "소지금이 부족합니다." << endl;
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
		printCentered("[판매하기]\n\n");
		printCentered("1. 장비\n");
		printCentered("2. 소비\n");
		printCentered("3. 기타\n");
		printCentered("4. 뒤로가기\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			shopMenu_sellItem(ItemType::장비);
			continue;

		case 2:
			system("cls");
			shopMenu_sellItem(ItemType::소비);
			continue;

		case 3:
			system("cls");
			shopMenu_sellItem(ItemType::기타);
			continue;

		case 4:
			return;

		default:
			printCentered("잘못된 입력입니다.\n");
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
		cout << "판매하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			if (type == ItemType::장비 && player->getEquipped(sel))
			{
				cout << "장착중인 아이템입니다." << endl;
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
		printCentered("[인벤토리 메뉴]\n\n");
		printCentered("1. 인벤토리 확인\n");
		printCentered("2. 장착하기\n");
		printCentered("3. 해제하기\n");
		printCentered("4. 뒤로가기\n");

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
			cout << "잘못된 입력입니다." << endl;
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
			player->printInventory(ItemType::장비);
			break;

		case 2:
			player->printInventory(ItemType::소비);
			break;

		case 3:
			player->printInventory(ItemType::기타);
			break;

		case 4:
			return;

		default:
			cout << "잘못된 입력입니다." << endl;
			break;
		}
		cout << "소지금 : " << player->getMoney() << endl;
		cout << "1 : 장비\t2 : 소비\t3 : 기타\t4 : 뒤로가기" << endl;
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
		player->printInventory(ItemType::장비);
		cout << "장착하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::장비) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			EquipItem* item = dynamic_cast<EquipItem*>(itemdb.getItem(player->getItem(ItemType::장비,sel)));


			if (player->getEquipped(sel))
			{
				cout << item->getName() << "은 이미 장착중입니다." << endl;
			}
			else if (!player->equippedCheck(item))
			{
				player->setEquipped(sel, true);
				cout << item->getName() << "을 장착했습니다." << endl;
				player->equip(item);

				client->Send("EquipItem");
				client->Send(userId);
				client->Send(std::to_string(player->getItem(ItemType::장비,sel)));
				client->Send("1");
			}
			else
			{
				std::string type = "ERROR";
				switch (item->getEquipType())
				{
				case EquipType::투구:
					type = "투구";
					break;
				case EquipType::상의:
					type = "상의";
					break;
				case EquipType::하의:
					type = "하의";
					break;
				case EquipType::장갑:
					type = "장갑";
					break;
				case EquipType::신발:
					type = "신발";
					break;
				case EquipType::무기:
					type = "무기";
					break;
				}
				cout << type << "은(는) 이미 장착중입니다." << endl;
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
		player->printInventory(ItemType::장비);
		cout << "장착 해제하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::장비) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			EquipItem* item = dynamic_cast<EquipItem*>(itemdb.getItem(player->getItem(ItemType::장비,sel)));

			if (player->getEquipped(sel))
			{
				player->setEquipped(sel,false);
				cout << item->getName() << "을 장착해제 했습니다." << endl;
				player->unequip(item);

				client->Send("EquipItem");
				client->Send(userId);
				client->Send(std::to_string(player->getItem(ItemType::장비,sel)));
				client->Send("0");
			}
			else
			{
				cout << item->getName() << "은 장착중이 아닙니다." << endl;
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
	cout << "메인 메뉴로 돌아갑니다..." << endl;
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
		printCentered("1. 초보자 사냥터\n");
		printCentered("2. 중급자 사냥터\n");
		printCentered("3. 상급자 사냥터\n");
		printCentered("4. 메인 메뉴\n");

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
			cout << "잘못된 입력입니다." << endl;
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
		cout << "[player의 턴]\n" << endl;

		cout << "[적 정보]" << endl;
		monster->printInfo();
		cout << endl;

		cout << "[나의 정보]" << endl;
		player->printStatus();
		cout << endl;

		int sel;

		cout << "1. 행동하기" << endl;
		cout << "2. 소비아이템 사용하기" << endl;

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			cout << "[player의 턴]\n" << endl;

			cout << "[적 정보]" << endl;
			monster->printInfo();
			cout << endl;

			cout << "[나의 정보]" << endl;
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
			cout << "잘못된 입력입니다." << endl;
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
		player->printInventory(ItemType::소비);
		cout << "사용하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(ItemType::소비) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
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
		cout << "player 사망!" << endl;
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
		printCentered("[거래소 메뉴]\n\n");
		printCentered("1. 구입\n");
		printCentered("2. 등록\n");
		printCentered("3. 등록 취소\n");
		printCentered("4. 대금 정산\n");
		printCentered("5. 뒤로가기\n");

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
			printCentered("잘못된 입력입니다.\n");
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
		printCentered("[거래하기]\n\n");
		printCentered("1. 장비\n");
		printCentered("2. 소비\n");
		printCentered("3. 기타\n");
		printCentered("4. 뒤로가기\n");

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
			printCentered("잘못된 입력입니다.\n");
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
		cout << "현재 소지금 : " << player->getMoney() << endl;
		cout << "구입하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > tradeShop->getListSize() || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
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
				cout << "소지금이 부족합니다." << endl;
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
					cout << itemdb.getItem(itemId)->getName() << "구입완료" << endl;
					cout << "남은 소지금 : " << player->getMoney() << endl;
				}
				else
				{
					cout << "이미 판매된 상품입니다." << endl;
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
		printCentered("[거래 등록 하기]\n\n");
		printCentered("1. 장비\n");
		printCentered("2. 소비\n");
		printCentered("3. 기타\n");
		printCentered("4. 뒤로가기\n");

		sel = _getch() - '0';

		switch (sel)
		{
		case 1:
			system("cls");
			tradeMenu_sellItem(ItemType::장비);
			break;

		case 2:
			system("cls");
			tradeMenu_sellItem(ItemType::소비);
			break;

		case 3:
			system("cls");
			tradeMenu_sellItem(ItemType::기타);
			break;

		case 4:
			return;

		default:
			printCentered("잘못된 입력입니다.\n");
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
		cout << "판매하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > player->getInventorySize(type) || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
			Sleep(1000);
		}
		else if (sel == 0)
		{
			cursorView(false);
			return;
		}
		else
		{
			if (type == ItemType::장비 && player->getEquipped(sel))
			{
				cout << "장착중인 아이템입니다." << endl;
			}
			else
			{
				cout << "판매하실 아이템의 가격을 입력하세요 : ";
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
		cout << "현재 소지금 : " << player->getMoney() << endl;
		cout << "거래 등록을 취소하실 아이템의 번호를 입력하세요(0 입력시 뒤로가기) : ";

		std::cin >> sel;

		if (cin.fail() || sel > tradeShop->getListSize() || sel < 0)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "잘못된 입력입니다." << endl;
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
				cout << itemdb.getItem(itemId)->getName() << "거래 등록 취소 완료" << endl;
				cout << "남은 소지금 : " << player->getMoney() << endl;
			}
			else
			{
				cout << "이미 판매된 상품입니다." << endl;
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
		printCentered("정산받을 대금이 없습니다.");
		Sleep(1500);
	}
	else
	{
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
		printCentered(responce + "의 정산 대금을 받았습니다.");
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
	printCentered("[순위표]\n");
	cout << endl << endl;
	for (int i = 0; i < count; i++)
	{
		std::string id = client->Receive();
		std::string level = client->Receive();
		printCentered("RANK : " + std::to_string(i + 1) + " / LEVEL : " + level + " / ID : " + id);
		cout << endl << endl;
	}

	cout << endl;
	blinkText(paddingCenter("뒤로 돌아가려면 아무 키나 누르세요"),500);
}

void TRPG_Game::monsterTurn()
{
	int atkdamage = 0, pureDamage = 0;

	system("cls");
	cout << "[Monster의 턴]\n" << endl;

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
		cout << "적 처치 성공!" << endl;
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