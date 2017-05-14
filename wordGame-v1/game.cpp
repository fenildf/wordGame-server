#include "game.h"
#include <conio.h>
#include <iostream>
#include<fstream>
#include <string>
using namespace std;

game::game():currentPlayer(nullptr),mode(FAIL)
{
	loadChallenger();
	
	loadDesigner();

	player::allRankInit();
}

game::~game()
{
	player::saveAllRank();
	saveChallenger();
	saveDesigner();
	
	auto cbegin = ChallengerInfo.begin(), cend = ChallengerInfo.end();
	while (cbegin != cend) { delete cbegin->second; cbegin++; }
	auto dbegin = designerInfo.begin(), dend = designerInfo.end();
	while (dbegin != dend) { delete dbegin->second; dbegin++; }
}

void game::run()
{
	cout << "*************************************************************" << endl;
	cout << "*                                                           *" << endl;
	cout << "*                     �� �� �� �� �� Ϸ                     *" << endl;
	cout << "*                                                           *" << endl;
	cout << "*************************************************************" << endl;
	cout << endl;
	
	int option=userIn();
	while (option != QUIT)
	{
		if (mode == CHALLENGE) option = challengeUi();
		else if (mode == DESIGN) option = designUi();

		if (option == LOGOUT) option = userIn();
	}
}

Mode game::Login()
{
	string name, pw;
	string type;
	map<string, player*> *user = nullptr;

	cout << "ѡ���¼����(0.����\t1.����\t2.����):";
	cin >> type;
	while (type != "0"&&type != "1"&&type != "2")
	{
		cout << "ѡ���¼����(0.����\t1.����\t2.����):";
		cin >> type;
	}
	if (type == "2") return FAIL;
	else if (stoi(type) == CHALLENGE) user = &ChallengerInfo;
	else user = &designerInfo;
	cout << "�������û���(����\"r\"����)��";
	cin >> name;
	while (name!="r")
	{
		auto target = (*user).find(name);
		if (target != (*user).end())
		{
			cout << "���������룺";
			cin >> pw;
			while (pw!="r"&&pw != target->second->getPw())
			{
				cout << "�������" << endl;
				cout << "����������(����\"r\"����)��";
				cin >> pw;
			}
			if (pw == "r") name = "r";
			else {
				currentPlayer = target->second;
				currentPlayer->getAllUser(&ChallengerInfo,&designerInfo);
				return Mode(stoi(type));
			}
		}
		else
		{
			cout << "�Ҳ������û���" << endl;
			cout << "�������û���(����\"r\"����)��";
			cin >> name;
		}
	}
	return FAIL;
}

Mode game::Register()
{
	string name, pw;
	string type;
	map<string, player*> *user = nullptr;

	cout << "ѡ��ע������(0.����\t1.����):";
	cin >> type;
	while (type != "0"&&type != "1"&&type != "2")
	{
		cout << "ѡ��ע������(0.����\t1.����\t2.����):";
		cin >> type;
	}
	if (type == "2") return FAIL;
	else if (stoi(type) == CHALLENGE) user = &ChallengerInfo;
	else user = &designerInfo;

	cout << "�������û���(����\"r\"����)��";
	cin >> name;
	while (name != "r")
	{
		auto target = (*user).find(name);
		if (target == (*user).end())
		{
			cout << "���������룺";
			cin >> pw;
			while (pw.size() == 0)
			{
				cout << "����������(����\"r\"����)��";
				cin >> pw;
			}
			if (pw == "r") name = "r";
			else {
				player *new_player = nullptr;
				if (stoi(type) == CHALLENGE)
				{
					new_player = new challenger(name, pw);
					challenger::addChallenger(name);
				}
				else
				{
					new_player = new designer(name, pw);
					designer::addDesigner(name);
				}
				(*user).insert(make_pair(name, new_player));
				currentPlayer = new_player;
				currentPlayer->getAllUser(&ChallengerInfo,&designerInfo);
				return Mode(stoi(type));
			}
		}
		else
		{
			cout << "�û��Ѵ��ڣ�" << endl;
			cout << "�������û���(����\"r\"����)��";
			cin >> name;
		}
	}
	return FAIL;
}

int game::userIn()
{
	string option;
	cout << "��ѡ��0.��¼\t1.ע��\t2.�˳�" << endl;
	cin >> option;
	int op = 0;
	while (mode == FAIL&&op != QUIT)
	{
		while (option!="0"&&option != "1"&&option != "2")
		{
			cout << "��������ȷ��ѡ�0.��¼\t1.ע��\t2.�˳�" << endl;
			cin >> option;
		}
		if (option == "2") op = QUIT;
		else op = stoi(option);

		switch (op)
		{
		case LOGIN:
			cout << "************************�� ¼**************************" << endl;
			mode = Login();
			break;
		case REGISTER:
			cout << "*************************ע ��*************************" << endl;
			mode = Register();
			break;
		case QUIT:
			break;
		default:
			break;
		}
	}
	return op;
}

int game::challengeUi()
{
	string choice;
	cout << "*************************�� ��*************************" << endl;
	cout << "��ѡ��0.�鿴����\t1.�����û�\t2.ע��\t3.�˳�" << endl;
	cin >> choice;
	while (choice!="2"&&choice!="3")
	{
		if (choice == "0") rank();
		else if (choice == "1") search();

		cout << "��ѡ��0.�鿴����\t1.�����û�\t2.ע��\t3.�˳�" << endl;
		cin >> choice;
	}
	if (choice == "2")
	{
		mode = FAIL;
		return LOGOUT;
	}
	else if (choice == "3") return QUIT;
	cout << "******************************************************" << endl;
}

void game::rank()
{
	cout << "*************************�� ��*************************" << endl;
	cout << "��ѡ��0.�ҵ�����\t1.��������\t2.����" << endl;
	string choice;
	cin >> choice;
	while (choice!="2")
	{
		if (choice=="0")
		{
			currentPlayer->rank(currentPlayer->getName(),mode);
		}
		else if(choice=="1")
		{
			string name;
			string type;
			cout << "����(0.������\t1.������\t2.����):";
			cin >> type;
			while (type != "0"&&type != "1"&&type != "2")
			{
				cout << "����(0.������\t1.������\t2.����):";
				cin >> type;
			}
			if (type != "2")
			{
				cout << "�û���:";
				cin >> name;
				if (!currentPlayer->rank(name, Mode(stoi(type)))) cout << "�û������ڣ�" << endl;
			}
		}
		cout << "��ѡ��0.�ҵ�����\t1.��������\t2.����" << endl;
		cin >> choice;
	}
	cout << "******************************************************" << endl;
}

void game::search()
{
	string choice;
	string attr;
	string aChoice;
	cout << "**********************�� �� �� ��**********************" << endl;
	cout << "����(0.������\t1.������\t2.����):";
	cin >> choice;
	while (choice!="2")
	{
		if (choice == "0")
		{
			cout << "����(0.����\t1.�ȼ�\t2.������\t3.����):";
			cin >> aChoice;
			while (aChoice != "0"&&aChoice != "1"&&aChoice != "2"&&aChoice != "3")
			{
				cout << "����������(0.����\t1.�ȼ�\t2.������\t3.����):";
				cin >> aChoice;
			}
			switch (stoi(aChoice))
			{
			case 0:
				cout << "����:";
				cin >> attr;
				searchChallenger(attr, -1, -1, -1);
				break;
			case 1:
				cout << "�ȼ�:";
				cin >> attr;
				searchChallenger(string(), stoi(attr), -1, -1);
				break;
			case 2:
				cout << "������:";
				cin >> attr;
				searchChallenger(string(), -1, stoi(attr), -1);
				break;
			case 3:
				cout << "����:";
				cin >> attr;
				searchChallenger(string(), -1, -1, stoi(attr));
				break;
			default:
				break;
			}
		}
		else if (choice == "1")
		{
			cout << "����(0.����\t1.�ȼ�\t2.������):";
			cin >> aChoice;
			while (aChoice != "0"&&aChoice != "1"&&aChoice != "2")
			{
				cout << "����������(0.����\t1.�ȼ�\t2.������):";
				cin >> aChoice;
			}
			switch (stoi(aChoice))
			{
			case 0:
				cout << "����:";
				cin >> attr;
				searchDesigner(attr, -1, -1);
				break;
			case 1:
				cout << "�ȼ�:";
				cin >> attr;
				searchDesigner(string(), stoi(attr), -1);
				break;
			case 2:
				cout << "������:";
				cin >> attr;
				searchDesigner(string(), -1, stoi(attr));
				break;
			default:
				break;
			}
		}
		cout << "����(0.������\t1.������\t2.����):";
		cin >> choice;
	}
	cout << "******************************************************" << endl;
}

void game::searchChallenger(string name, int level, int exp, int pass)
{
	auto begin = ChallengerInfo.begin(), end = ChallengerInfo.end();
	bool flag = false;
	if (name != string())
	{
		while (begin!=end)
		{
			if (begin->first == name)
			{
				begin->second->showInfo();
				flag = true;
				return;
			}
			else
			{
				begin++;
			}
		}
	}
	else if(level!=-1)
	{
		while (begin!=end)
		{
			if (begin->second->getLevel() == level)
			{
				begin->second->showInfo();
				flag = true;
			}
			begin++;
		}
	}
	else if (pass != -1)
	{
		while (begin != end)
		{
			if (begin->second->getPass() == pass)
			{
				begin->second->showInfo();
				flag = true;
			}
			begin++;
		}
	}
	else if (exp != -1)
	{
		while (begin != end)
		{
			if (begin->second->getExp() == exp)
			{
				begin->second->showInfo();
				flag = true;
			}
			begin++;
		}
	}
	if (!flag) cout << "�û������ڣ�" << endl;
}

void game::searchDesigner(string name, int level, int word)
{
	auto begin = designerInfo.begin(), end = designerInfo.end();
	bool flag = false;
	if (name != string())
	{
		while (begin != end)
		{
			if (begin->first == name)
			{
				begin->second->showInfo();
				flag = true;
				return;
			}
			else
			{
				begin++;
			}
		}
	}
	else if (level != -1)
	{
		while (begin != end)
		{
			if (begin->second->getLevel() == level)
			{
				begin->second->showInfo();
				flag = true;
			}
			begin++;
		}
	}
	else if (word != -1)
	{
		while (begin != end)
		{
			if (begin->second->getWord() == word)
			{
				begin->second->showInfo();
				flag = true;
			}
			begin++;
		}
	}
	if (!flag) cout << "�û������ڣ�" << endl;
}

int game::designUi()
{
	string choice;
	cout << "��ѡ��0.�鿴����\t1.�����û�\t2.ע��\t3.�˳�" << endl;
	cin >> choice;
	while (choice != "2" && choice != "3")
	{
		if (choice == "0") rank();
		else if (choice == "1") search();

		cout << "��ѡ��0.�鿴����\t1.�����û�\t2.ע��\t3.�˳�" << endl;
		cin >> choice;
	}
	if (choice == "2")
	{
		mode = FAIL;
		return LOGOUT;
	}
	else if (choice == "3") return QUIT;
}

void game::loadChallenger()
{
	ifstream in("challenger.txt");
	string name, password;
	int level, pass, exp;
	while (!in.eof())
	{
		in >> name >> password >> level >> pass >> exp;
		ChallengerInfo[name] = new challenger(name, password, level, pass, exp);
	}
	in.close();
}

void game::loadDesigner()
{
	ifstream in("designer.txt");
	string name, password;
	int level, word;
	while (!in.eof())
	{
		in >> name >> password >> level >> word;
		designerInfo[name] = new designer(name, password, level, word);
	}
	in.close();
}

void game::saveChallenger()
{
	ofstream out("challenger.txt");
	auto begin = ChallengerInfo.begin(), end = ChallengerInfo.end();

	while (begin!=end)
	{
		out << begin->second->getName() << " " << begin->second->getPw() << " " << begin->second->getLevel()<<" "
			<< begin->second->getPass() << " " << begin->second->getExp() << endl;
		begin++;
	}
	out.close();
}

void game::saveDesigner()
{
	ofstream out("designer.txt");
	auto begin = designerInfo.begin(), end = designerInfo.end();

	while (begin != end)
	{
		out << begin->second->getName() << " " << begin->second->getPw() << " " << begin->second->getLevel()<<" "
			<< begin->second->getWord() << endl;
		begin++;
	}
	out.close();
}
