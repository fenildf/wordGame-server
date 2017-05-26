#include "player.h"
#include <iostream>
#include <fstream>
using namespace std;

vector<string> player::cLevelRanking = vector<string>();
vector<string> player::dLevelRanking = vector<string>();
//vector<string> player::expRanking = vector<string>();
//vector<string> player::passRanking = vector<string>();
//vector<string> player::puzzleRanking = vector<string>();
vector<int> challenger::levelExp = vector<int>();
vector<int> designer::levelPuzzle = vector<int>();
player::player(const player & p):name(p.name),password(p.password),level(p.level),clgInfo(p.clgInfo),dsInfo(p.dsInfo)
{

}

player & player::operator=(const player & p)
{
	name = p.name;
	password = p.password;
	level = p.level;
	clgInfo = p.clgInfo;
	dsInfo = p.dsInfo;
	
	return *this;
}


void player::rankInit(vector<string> &ranking,string file)
{
	ifstream in(file);
	
	string name;
	while (in>>name)
	{
		ranking.push_back(name);
	}
	in.close();
}

void player::saveRank(vector<string> ranking, string file)
{
	ofstream out(file);

	for (auto n:ranking)
	{
		out << n << endl;
	}
}

inline
challenger::challenger(const challenger & c):player(c),exp(c.exp),pass(c.pass)
{
}

challenger & challenger::operator=(challenger & c)
{
	player::operator=(c);
	exp = c.exp;
	pass = c.pass;
	return *this;
}

challenger::~challenger()
{
}



bool challenger::solve(string puzzle)
{
	string ans;
	cin >> ans;
	if (ans != puzzle) cout << "错误！" << endl;
	return ans==puzzle;
}

int challenger::rank( Mode m)
{
	int lr = 0;
	if (m == CHALLENGE)
	{
		int i = 0;
		for (; i < cLevelRanking.size() && (lr == 0); i++)
		{
			if (cLevelRanking[i] == name) lr = i + 1;
		}
	}
	else
	{
		int i = 0;
		for (; i < dLevelRanking.size() && (lr == 0); i++)
		{
			if (dLevelRanking[i] == name) lr = i + 1;

		}
	}
	return lr;
}

bool challenger::showInfo(char recvbuf[])
{
	cout << "姓名\t" << "等级\t" << "闯关数\t" << "经验" << endl;
	//cout << name << "\t" << level << "\t" << pass << "\t" << exp << "\t" << endl;
	cout << endl;
	int r = rank(CHALLENGE);
	sprintf(recvbuf, "%s %d %d %d %d", name.c_str(), level, pass, exp,r);
	send(currentSocket, recvbuf, strlen(recvbuf) + 1, 0);

	//rank(name, CHALLENGE);
	cout << "**************************************************************" << endl;
	return true;
}

void challenger::reRank()
{
	
	int i = 0;
	while (cLevelRanking.size() > i&&cLevelRanking[i] != name) i++;
	if (i != cLevelRanking.size())
	{
		string next;
		while (i >= 1)
		{
			next = cLevelRanking[i - 1];
			if (level < (*clgInfo)[next]->getLevel() || (level == (*clgInfo)[next]->getLevel() && pass<(*clgInfo)[next]->getPass())
				||(level == (*clgInfo)[next]->getLevel() && pass==(*clgInfo)[next]->getPass()&&exp<(*clgInfo)[next]->getExp())) break;
			swap(cLevelRanking[i - 1], cLevelRanking[i]);
			i--;
		}
	}
}

void challenger::refreshInfo(int d)
{
	exp += d;
	if (level < MAX_LEVEL&&exp >= levelExp[level + 1]) level++;
	if (d > 0) pass++;
}

void challenger::addChallenger(string n)
{
	player::cLevelRanking.push_back(n);
	//player::passRanking.push_back(n);
	//player::expRanking.push_back(n);
}

void challenger::levelExpInit()
{
	levelExp.push_back(0);
	for (int i = 1; i < 100; i++)
	{
		levelExp.push_back(i*5 + levelExp[i - 1]);
	}
}

int player::getAttr(ATTR a)
{
	switch (a)
	{
	case LEVEL:return getLevel();
		break;
	case PASS:return getPass();
		break;
	case EXP:return getExp();
		break;
	case PUZZLE:return getPuzzle();
		break;
	default:return 0;
		break;
	}
}

void player::allRankInit()
{
	player::rankInit(cLevelRanking, "cLevel.txt");
	player::rankInit(dLevelRanking, "dLevel.txt");
}

void player::saveAllRank()
{
	saveRank(cLevelRanking, "cLevel.txt");
	saveRank(dLevelRanking, "dLevel.txt");
}

void player::attrReRank(vector<string>& ranking, int attr,Mode m,ATTR a)
{
	int i = 0;
	map<string, player*> *user = nullptr;
	if (m == CHALLENGE) user = clgInfo;
	else user = dsInfo;
	while (ranking.size() > i&&ranking[i] != name) i++;
	if (i != ranking.size())
	{
		string next;
		while (i>=1)
		{
			next = ranking[i - 1];
			if (attr < (*user)[next]->getAttr(a)) break;
			swap(ranking[i - 1], ranking[i]);
			i--;
		}
	}
}

int designer::rank(Mode m)
{
	int lr = 0;
	if (m == CHALLENGE)
	{
		int i = 0;
		for (; i < cLevelRanking.size() && (lr == 0); i++)
		{
			if (cLevelRanking[i] == name) lr = i + 1;
		}
	}
	else
	{
		int i = 0;
		for (; i < dLevelRanking.size() && (lr == 0); i++)
		{
			if (dLevelRanking[i] == name) lr = i + 1;
			
		}
	}
	return lr;
}

void designer::refreshInfo(int d)
{
	puzzle += d;
	if (level < MAX_LEVEL&&puzzle >= levelPuzzle[level + 1]) level++;
}

bool designer::showInfo(char recvbuf[])
{
	cout << "姓名\t" << "等级\t" << "出题数" << endl;
	//cout << name << "\t" << level << "\t" << puzzle << endl;
	cout << endl;
	int r = rank(DESIGN);
	sprintf(recvbuf, "%s %d %d %d", name.c_str(), level, puzzle,r);
	send(currentSocket, recvbuf, strlen(recvbuf) + 1, 0);

	//rank(name, DESIGN);
	cout << "**************************************************************" << endl;
	return true;
}

void designer::reRank()
{
	int i = 0;
	while (dLevelRanking.size() > i&&dLevelRanking[i] != name) i++;
	if (i != dLevelRanking.size())
	{
		string next;
		while (i >= 1)
		{
			next = dLevelRanking[i - 1];
			if (level < (*dsInfo)[next]->getLevel()|| (level == (*dsInfo)[next]->getLevel()&&puzzle<(*dsInfo)[next]->getPuzzle())) break;
			swap(dLevelRanking[i - 1], dLevelRanking[i]);
			i--;
		}
	}
}


void designer::addDesigner(string n)
{
	player::dLevelRanking.push_back(n);
}
