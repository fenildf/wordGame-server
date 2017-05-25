#include "game.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <thread>
#include "ClientThread.h"
using namespace std;
void matchJudge(player *c1, player *c2);

game::game(std::map<std::string, player*> *cli, std::map<std::string, player*> *dsi, std::map<int, std::vector<std::string>> *vc, SOCKET sSocket, SOCKET cSocket) :
	ChallengerInfo(cli),designerInfo(dsi),vocabulary(vc), currentPlayer(nullptr),mode(FAIL),serverSocket(sSocket),clientSocket(cSocket)
{
	//loadChallenger();
	
	//loadDesigner();

	//loadVocabulary();
	//player::allRankInit();
	//challenger::levelExpInit();
	//designer::levelPuzzleInit();
}

game::~game()
{
	/*player::saveAllRank();
	saveChallenger();
	saveDesigner();
	saveVocabulary();
	auto cbegin = ChallengerInfo->begin(), cend = ChallengerInfo->end();
	while (cbegin != cend) { delete cbegin->second; cbegin++; }
	auto dbegin = designerInfo->begin(), dend = designerInfo.end();
	while (dbegin != dend) { delete dbegin->second; dbegin++; }*/
}

void game::run()
{
	cout << "*************************************************************" << endl;
	cout << "*                                                           *" << endl;
	cout << "*                     单 词 消 除 游 戏                     *" << endl;
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

	cout << "选择登录类型(0.闯关\t1.出题\t2.返回):";
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	type = recvbuf;
	send(clientSocket, "1", 2, 0);
	if (type == "2") return FAIL;
	else if (stoi(type) == CHALLENGE) user = ChallengerInfo;
	else user = designerInfo;
	cout << "请输入用户名(输入\"r\"返回)：";
	
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	name = recvbuf;
	while (name!="r")
	{
		auto target = (*user).find(name);
		if (target != (*user).end())
		{
			sprintf(recvbuf, "%d", 1);
			//send(clientSocket, recvbuf, MAX_BUFLEN, 0);
			cout << "请输入密码：";
			
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			pw = recvbuf;
			
			while (pw!="r"&&pw != target->second->getPw())
			{
				sprintf(recvbuf, "%d", 0);
				send(clientSocket, recvbuf, MAX_BUFLEN, 0);

				cout << "密码错误！" << endl;
				cout << "请输入密码(输入\"r\"返回)：";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				pw = recvbuf;
			}
			if (pw == "r") name = "r";
			else {
				sprintf(recvbuf, "%d", 1);
				send(clientSocket, recvbuf, strlen(recvbuf)+1, 0);

				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				
				currentPlayer = target->second;
				currentPlayer->setSocket(clientSocket);
				currentPlayer->setOnline();
				if (stoi(type) == CHALLENGE)
				{
					sprintf(recvbuf, "%s %s %d %d %d", currentPlayer->getName().c_str(), currentPlayer->getPw().c_str(), currentPlayer->getLevel(), currentPlayer->getPass(), currentPlayer->getExp());
				}
				else
				{
					sprintf(recvbuf, "%s %s %d %d", currentPlayer->getName().c_str(), currentPlayer->getPw().c_str(), currentPlayer->getLevel(), currentPlayer->getPuzzle());
				}
				send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);

				currentPlayer->getAllUser(ChallengerInfo,designerInfo);
				return Mode(stoi(type));
			}
		}
		else
		{
			sprintf(recvbuf, "%d", 0);
			send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
			cout << "找不到该用户！" << endl;
			cout << "请输入用户名(输入\"r\"返回)：";
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			name = recvbuf;
		}
	}
	return FAIL;
}

Mode game::Register()
{
	string name, pw;
	string type;
	map<string, player*> *user = nullptr;

	cout << "选择注册类型(0.闯关\t1.出题\t2.返回):";
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	type = recvbuf;
	/*while (type != "0"&&type != "1"&&type != "2")
	{
		cout << "选择注册类型(0.闯关\t1.出题\t2.返回):";
		cin >> type;
	}*/
	if (type == "2") return FAIL;
	else if (stoi(type) == CHALLENGE) user = ChallengerInfo;
	else user = designerInfo;

	cout << "请输入用户名(输入\"r\"返回)：";
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	name = recvbuf;
	while (name != "r")
	{
		auto target = (*user).find(name);
		if (target == (*user).end())
		{
			sprintf(recvbuf, "%d", 1);
			send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
			cout << "请输入密码：";
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			pw = recvbuf;
			/*
			while (pw.size() == 0)
			{
				cout << "请输入密码(输入\"r\"返回)：";
				cin >> pw;
			}*/
			if (pw == "r") name = "r";
			else {
				sprintf(recvbuf, "%d", 1);
				send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
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
				currentPlayer->getAllUser(ChallengerInfo,designerInfo);
				currentPlayer->setSocket(clientSocket);
				currentPlayer->setOnline();
				return Mode(stoi(type));
			}
		}
		else
		{
			sprintf(recvbuf, "%d", 0);
			send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
			cout << "用户已存在！" << endl;
			cout << "请输入用户名(输入\"r\"返回)：";
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			name = recvbuf;
		}
	}
	return FAIL;
}

int game::userIn()
{
	string option;
	cout << "请选择：0.登录\t1.注册\t2.退出" << endl;
	//cin >> option;
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	option = recvbuf;
	int op = 0;
	while (mode == FAIL&&op != QUIT)
	{
		if (option == "2") op = QUIT;
		else op = stoi(option);

		switch (op)
		{
		case LOGIN:
			cout << "************************登 录**************************" << endl;
			mode = Login();
			break;
		case REGISTER:
			cout << "*************************注 册*************************" << endl;
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
	
	sprintf(recvbuf, "%d", currentPlayer->wasCha);
	send(clientSocket, recvbuf, strlen(recvbuf)+1, 0);
	if (currentPlayer->wasCha)
	{
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		if (recvbuf[0] == 'y')
		{
			currentPlayer->reply = 1;
			currentPlayer->isNew = 1;
		}
		else
		{
			currentPlayer->reply = 0;
			currentPlayer->isNew = 0;
		}
		Sleep(1000);
		while (currentPlayer->over != 1) continue;
	}
	string choice;
	cout << "*************************闯 关 者 界 面*************************" << endl;
	cout << "请选择：0.开始游戏\t1.查看排名\t2.查找用户\t3.注销\t4.退出" << endl;
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	choice = recvbuf;
	while (choice!="3"&&choice!="4")
	{
		if (choice == "0") challenge();
		else if (choice == "1") rank();
		else if (choice == "2") search();

		sprintf(recvbuf, "%d", currentPlayer->wasCha);
		send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
		if (currentPlayer->wasCha)
		{
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			if (recvbuf[0] == 'y')
			{
				currentPlayer->reply = 1;
				currentPlayer->isNew = 1;
			}
			else
			{
				currentPlayer->reply = 0;
				currentPlayer->isNew = 0;
			}
			Sleep(1000);
			while (currentPlayer->over != 1) continue;
		}

		cout << "请选择：0.开始游戏\t1.查看排名\t2.查找用户\t3.注销\t4.退出" << endl;
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		choice = recvbuf;
	}
	if (choice == "3")
	{
		mode = FAIL;
		return LOGOUT;
	}
	else if (choice == "4") return QUIT;
	cout << "******************************************************" << endl;
}

void game::challenge()
{
	//system("cls");

	string currentWord;
	auto begin = vocabulary->begin(), end = vocabulary->end();
	int pass = 0, solved = 0;
	int duration = 10000;
	default_random_engine e(time(0));
	uniform_int_distribution<unsigned> u(0, 100);
	
	string again = "y";
	while (again=="y")
	{
		
		cout << "************************第 " << pass + 1 << " 关************************" << endl;
		currentWord = begin->second[u(e) % begin->second.size()];
		//cout << currentWord << endl;
		//_sleep(duration);
		//system("cls");
		send(clientSocket, currentWord.c_str(), currentWord.size() + 1, 0);

		cout << "************************第 " << pass + 1 << " 关************************" << endl;
		cout << "输入答案:" << endl;
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		sscanf(recvbuf, "%d", &solved);
		while (solved)
		{
			pass++;
			begin++;
			if (begin == end) break;

			currentPlayer->refreshInfo(((pass + 1) % 5 + 1) * 10);
			currentPlayer->reRank();
			currentWord = begin->second[u(e) % begin->second.size()];
			//system("cls");

			cout << "************************第 " << pass + 1 << " 关************************" << endl;
			send(clientSocket, currentWord.c_str(), currentWord.size() + 1, 0);
			if ((pass - 1) % 5 == 0 && duration >= 1000) duration -= 500;
			//_sleep(duration);
			//system("cls");
			cout << "************************第 " << pass + 1 << " 关************************" << endl;
			cout << "输入答案:" << endl;
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			sscanf(recvbuf, "%d", &solved);
		}
		if (begin == end)
		{
			cout << "恭喜全部通关！" << endl;
			send(clientSocket, "end!", 5, 0);
		}
		currentPlayer->showInfo(recvbuf);

		cout << "再来一次？y/n" << endl;
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		again = recvbuf;
		/*while (again!="y"&&again!="n")
		{
			cout << "y/n?" << endl;
			cin >> again;
		}*/
		pass = 0;
		duration = 0;
		begin = vocabulary->begin();
	}

	cout << "****************************************************" << endl;
}

void game::rank()
{
	cout << "*************************排 名*************************" << endl;
	cout << "请选择：0.我的排名\t1.他人排名\t2.返回" << endl;
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
			cout << "类型(0.闯关者\t1.出题者\t2.返回):";
			cin >> type;
			while (type != "0"&&type != "1"&&type != "2")
			{
				cout << "类型(0.闯关者\t1.出题者\t2.返回):";
				cin >> type;
			}
			if (type != "2")
			{
				cout << "用户名:";
				cin >> name;
				if (!currentPlayer->rank(name, Mode(stoi(type)))) cout << "用户不存在！" << endl;
			}
		}
		cout << "请选择：0.我的排名\t1.他人排名\t2.返回" << endl;
		cin >> choice;
	}
	cout << "******************************************************" << endl;
}

void game::search()
{
	string choice;
	string attr;
	string aChoice;
	string reply, name;
	cout << "**********************查 找 用 户**********************" << endl;
	cout << "类型(0.闯关者\t1.出题者\t2.返回):";
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	choice = recvbuf;
	while (choice!="2")
	{
		if (choice == "0")
		{
			cout << "属性(0.姓名\t1.等级\t2.闯关数\t3.经验\t4.在线):";
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			aChoice = recvbuf;
			
			switch (stoi(aChoice))
			{
			case 0:
				cout << "姓名:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;

				searchChallenger(attr, -1, -1, -1,0);
				break;
			case 1:
				cout << "等级:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchChallenger(string(), stoi(attr), -1, -1, 0);
				break;
			case 2:
				cout << "闯关数:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchChallenger(string(), -1, stoi(attr), -1, 0);
				break;
			case 3:
				cout << "经验:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchChallenger(string(), -1, -1, stoi(attr), 0);
				break;
			case 4:
				searchChallenger(string(), -1, -1, -1, 1);
				cout << "是否挑战?(y/n):";
				
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				reply = recvbuf;
				send(clientSocket, "ok", 3, 0);
				if (reply == "y")
				{
					cout << "挑战的用户名:" << endl;
					recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
					name = recvbuf;
					match(name);
				}
			default:
				break;
			}
		}
		else if (choice == "1")
		{
			cout << "属性(0.姓名\t1.等级\t2.出题数):";
			recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
			aChoice = recvbuf;
			
			switch (stoi(aChoice))
			{
			case 0:
				cout << "姓名:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchDesigner(attr, -1, -1,0);
				break;
			case 1:
				cout << "等级:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchDesigner(string(), stoi(attr), -1, 0);
				break;
			case 2:
				cout << "出题数:";
				recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
				attr = recvbuf;
				searchDesigner(string(), -1, stoi(attr),0);
				break;
			case 3:
				searchDesigner(string(), -1, -1,1);
				break;
			default:
				break;
			}
		}
		cout << "类型(0.闯关者\t1.出题者\t2.返回):";
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		choice = recvbuf;
	}
	cout << "******************************************************" << endl;
}

void game::searchChallenger(string name, int level, int exp, int pass,int online)
{
	auto begin = ChallengerInfo->begin(), end = ChallengerInfo->end();
	bool flag = false;
	vector<player*> temp;
	if (name != string())
	{
		while (begin!=end)
		{
			if (begin->first == name)
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	else if(level!=-1)
	{
		while (begin!=end)
		{
			if (begin->second->getLevel() == level)
			{
				temp.push_back(begin->second);
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
				temp.push_back(begin->second);
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
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	else if (online != 0)
	{
		while (begin != end)
		{
			if (begin->second->isOnline())
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}

	sprintf(recvbuf, "%d", temp.size());
	send(clientSocket, recvbuf, strlen(recvbuf)+1, 0);
	for (int i = 0; i < temp.size(); i++)
	{
		sprintf(recvbuf, "%s %d %d %d", temp[i]->getName().c_str(), temp[i]->getLevel(), temp[i]->getPass(), temp[i]->getExp());
		send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	}
}

void game::searchDesigner(string name, int level, int wordCount, int online)
{
	auto begin = designerInfo->begin(), end = designerInfo->end();
	bool flag = false;
	vector<player*> temp;
	if (name != string())
	{
		while (begin != end)
		{
			if (begin->first == name)
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	else if (level != -1)
	{
		while (begin != end)
		{
			if (begin->second->getLevel() == level)
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	else if (wordCount != -1)
	{
		while (begin != end)
		{
			if (begin->second->getPuzzle() == wordCount)
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	else if(online==1)
	{
		while (begin != end)
		{
			if (begin->second->isOnline())
			{
				temp.push_back(begin->second);
			}
			begin++;
		}
	}
	sprintf(recvbuf, "%d", temp.size());
	send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
	for (int i = 0; i < temp.size(); i++)
	{
		sprintf(recvbuf, "%s %d %d", temp[i]->getName().c_str(), temp[i]->getLevel(), temp[i]->getPuzzle());
		send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	}
}

int game::designUi()
{
	string choice;
	cout << "*************************出 题 者 界 面*************************" << endl;
	cout << "请选择：0.出题\t1.查看排名\t2.查找用户\t3.注销\t4.退出" << endl;
	//cin >> choice;
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	choice = recvbuf;
	while (choice != "4" && choice != "3")
	{
		if (choice == "0") design();
		else if (choice == "1") rank();
		else if (choice == "2") search();

		cout << "请选择：0.出题\t1.查看排名\t2.查找用户\t3.注销\t4.退出" << endl;
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		choice = recvbuf;
	}
	if (choice == "3")
	{
		mode = FAIL;
		return LOGOUT;
	}
	else if (choice == "4") return QUIT;
}

void game::design()
{
	//system("cls");
	string word;
	int flag = 1;
	cout << "*************************出 题*************************" << endl;
	cout << "输入要添加的新词（输入!q停止出题）:";
	recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
	word = recvbuf;
	while (word != "!q")
	{
		auto &list = (*vocabulary)[word.size()];
		if (list.size() == 0)
		{
			list.push_back(word);
		}
		else
		{
			for (int i = 0; i < list.size()&&flag!=0; i++)
			{
				if (list[i] == word) flag = 0;
			}
		}
		sprintf(recvbuf, "%d", flag);
		send(clientSocket, recvbuf, strlen(recvbuf) + 1, 0);
		if (flag)
		{
			cout << "添加成功！" << endl;
			currentPlayer->refreshInfo(1);
			currentPlayer->reRank();
		}
		else cout << "单词已存在！" << endl;

		cout << "输入要添加的新词（输入!q停止出题）:";
		recv(clientSocket, recvbuf, MAX_BUFLEN, 0);
		word = recvbuf;
	}
	//system("cls");
	currentPlayer->showInfo(recvbuf);
}
void game::match(string name)
{
	auto beg = ChallengerInfo->begin(), end = ChallengerInfo->end();
	while (beg!=end)
	{
		if (beg->first == name) break;
		beg++;
	}
	if (beg == end) send(clientSocket, "0", 2, 0);
	else if(!(beg->second->isOnline())) send(clientSocket, "1", 2, 0);
	else if (beg->second->busy) send(clientSocket, "2", 2, 0);
	else
	{
		beg->second->wasCha = 1;
		beg->second->opponent = currentPlayer->getName();
		while (!beg->second->isNew)
		{
			continue;
		}
		if (beg->second->reply == 1)
		{
			send(clientSocket, "4", 2, 0);
			currentPlayer->opponent = name;
			std::thread tr =std::thread(matchJudge, currentPlayer, beg->second);
			
			tr.join();
		}
		else
		{
			send(clientSocket, "3", 2, 0);
		}
		beg->second->isNew = 0;
	}
}
/*
void game::loadChallenger()
{
	ifstream in("challenger.txt");
	string name, password;
	int level, pass, exp;
	while (in>>name)
	{
		in >> password >> level >> pass >> exp;
		ChallengerInfo[name] = new challenger(name, password, level, pass, exp);
	}
	in.close();
}

void game::loadDesigner()
{
	ifstream in("designer.txt");
	string name, password;
	int level, word;
	while (in>>name)
	{
		in >> password >> level >> word;
		designerInfo[name] = new designer(name, password, level, word);
	}
	in.close();
}

void game::loadVocabulary()
{
	ifstream in("vocabulary.txt");
	string word;
	int l = 0;
	while (in>>word)
	{
		if (word.size() > l) l = word.size();
		vocabulary[l].push_back(word);
	}
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
			<< begin->second->getPuzzle() << endl;
		begin++;
	}
	out.close();
}

void game::saveVocabulary()
{
	ofstream out("vocabulary.txt");
	auto begin = vocabulary.begin(), end = vocabulary.end();
	while (begin!=end)
	{
		for (auto word:begin->second)
		{
			out << word << endl;
		}
		begin++;
	}
	out.close();
}*/

void matchJudge(player *c1, player *c2)
{
	c1->busy = 1, c2->busy = 1;
	c1->over = 0, c2->over = 0;
	char recvbuf1[512], recvbuf2[512];
	string currentWord;
	clock_t duration1, duration2, totalDura1 = 0, totalDura2 = 0;
	auto begin = vocabulary.begin(), end = vocabulary.end();
	int pass = 0, solved1 = 0, solved2 = 0;
	int win1, win2;
	int duration = 10000;
	default_random_engine e(time(0));
	uniform_int_distribution<unsigned> u(0, 100);

	{

		cout << "************************第 " << pass + 1 << " 关************************" << endl;
		currentWord = begin->second[u(e) % begin->second.size()];
		
		send(c1->currentSocket, currentWord.c_str(), currentWord.size() + 1, 0);
		send(c2->currentSocket, currentWord.c_str(), currentWord.size() + 1, 0);

		cout << "************************第 " << pass + 1 << " 关************************" << endl;
		cout << "输入答案:" << endl;
		recv(c1->currentSocket, recvbuf1, MAX_BUFLEN, 0);
		sscanf(recvbuf1, "%d %ld", &solved1, &duration1);
		totalDura1 += duration1;
		recv(c2->currentSocket, recvbuf2, MAX_BUFLEN, 0);
		sscanf(recvbuf2, "%d %ld", &solved2, &duration2);
		totalDura2 += duration2;
		while (solved1&&solved2&&pass!=9)
		{
			pass++;
			begin++;
			if (begin == end) break;
			
			if (duration1 < duration2)
			{
				c1->refreshInfo(((pass + 1) % 5 + 1) * 20);
				c1->reRank();
				c2->refreshInfo(-((pass + 1) % 5 + 1) * 5);
				c2->reRank();
			}
			else
			{
				c2->refreshInfo(((pass + 1) % 5 + 1) * 20);
				c2->reRank();
				c1->refreshInfo(-((pass + 1) % 5 + 1) * 5);
				c1->reRank();
			}
			currentWord = begin->second[u(e) % begin->second.size()];
			

			cout << "************************第 " << pass + 1 << " 关************************" << endl;
			send(c1->currentSocket, currentWord.c_str(), currentWord.size() + 1, 0);
			send(c2->currentSocket, currentWord.c_str(), currentWord.size() + 1, 0);
			if ((pass - 1) % 5 == 0 && duration >= 1000) duration -= 500;
			
			cout << "************************第 " << pass + 1 << " 关************************" << endl;
			cout << "输入答案:" << endl;
			recv(c1->currentSocket, recvbuf1, MAX_BUFLEN, 0);
			sscanf(recvbuf1, "%d %ld", &solved1, &duration1);
			recv(c2->currentSocket, recvbuf2, MAX_BUFLEN, 0);
			sscanf(recvbuf2, "%d %ld", &solved2, &duration2);
		}
		if (pass == 9)
		{
			if (totalDura1 < totalDura2)
			{
				win1 = 1;
				win2 = 0;
			}
			else
			{
				win1 = 1;
				win2 = 0;
			}
		}
		else if(solved1)
		{
			win1 = 1;
			win2 = 0;
		}
		else if(solved2)
		{
			win1 = 0;
			win2 = 1;
		}
		else if (totalDura1 < totalDura2)
		{
			win1 = 1;
			win2 = 0;
		}
		else
		{

			win1 = 0;
			win2 = 1;
		}
	}
	sprintf(recvbuf1, "%d", win1);
	sprintf(recvbuf2, "%d", win2);
	send(c1->currentSocket, recvbuf1, strlen(recvbuf1) + 1, 0);
	send(c2->currentSocket, recvbuf2, strlen(recvbuf2) + 1, 0);

	c1->busy = 0, c2->busy = 0;
	c1->over = 1, c2->over = 1;
	cout << "****************************************************" << endl;
}
