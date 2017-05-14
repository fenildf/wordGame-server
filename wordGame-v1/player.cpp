#include "player.h"
#include <iostream>
#include <fstream>
using namespace std;
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
vector<string> player::cLevelRanking = vector<string>();
vector<string> player::dLevelRanking = vector<string>();
vector<string> player::expRanking = vector<string>();
vector<string> player::passRanking = vector<string>();
vector<string> player::wordRanking = vector<string>();
vector<int> challenger::levelExp = vector<int>();
vector<int> designer::levelWord = vector<int>();

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



bool challenger::solve(string word)
{
	string ans;
	cin >> ans;
	while (ans != "q"&&ans != word)
	{
		cout << "错误！" << endl;
	}
	if (ans == word) cout << "正确！" << endl;
	return ans==word;
}

bool challenger::rank(string n, Mode m)
{
	if (m == CHALLENGE)
	{
		int lr = 0, pr = 0, er = 0, i = 0;
		for (; i < cLevelRanking.size() && (lr == 0 || pr == 0 || er == 0); i++)
		{
			if (cLevelRanking[i] == n) lr = i + 1;
			if (passRanking[i] == n) pr = i + 1;
			if (expRanking[i] == n) er = i + 1;
		}
		if (lr == 0 || pr == 0 || er == 0) return false;
		cout << "属性\t" << "等级\t" << "关卡\t" << "经验\t" << endl;
		cout << "排名\t" << lr << "\t" << pr << "\t" << er << "\t" << endl;
	}
	else
	{
		int lr = 0, wr=0, i = 0;
		for (; i < dLevelRanking.size() && (lr == 0 || wr == 0); i++)
		{
			if (dLevelRanking[i] == n) lr = i + 1;
			if (wordRanking[i] == n) wr = i + 1;
		}
		if (lr == 0 || wr == 0) return false;
		cout << "属性\t" << "等级\t" << "出题数\t" << endl;
		cout << "排名\t" << lr << "\t" << wr << "\t" << endl;
	}
	return true;
}

bool challenger::showInfo()
{
	cout << "姓名\t" << "等级\t" << "闯关数\t" << "经验" << endl;
	cout << name << "\t" << level << "\t" << pass << "\t" << exp << "\t" << endl;
	cout << endl;

	rank(name, CHALLENGE);
	cout << "**************************************************************" << endl;
	return true;
}

void challenger::addChallenger(string n)
{
	player::cLevelRanking.push_back(n);
	player::passRanking.push_back(n);
	player::expRanking.push_back(n);
}

void player::allRankInit()
{
	player::rankInit(cLevelRanking, "cLevel.txt");
	player::rankInit(dLevelRanking, "dLevel.txt");
	player::rankInit(passRanking, "pass.txt");
	player::rankInit(expRanking, "exp.txt");
	player::rankInit(wordRanking, "word.txt");
}

void player::saveAllRank()
{
	saveRank(cLevelRanking, "cLevel.txt");
	saveRank(dLevelRanking, "dLevel.txt");
	saveRank(passRanking, "pass.txt");
	saveRank(expRanking, "exp.txt");
	saveRank(wordRanking, "word.txt");
}

void challenger::expInc(int dExp)
{
	exp += dExp;
	if (level<=MAX_LEVEL&&exp >= levelExp[level + 1]) level++;
}

bool designer::rank(string n, Mode m)
{
	if (m == CHALLENGE)
	{
		int lr = 0, pr = 0, er = 0, i = 0;
		for (; i < cLevelRanking.size() && (lr == 0 || pr == 0 || er == 0); i++)
		{
			if (cLevelRanking[i] == n) lr = i + 1;
			if (passRanking[i] == n) pr = i + 1;
			if (expRanking[i] == n) er = i + 1;
		}
		if (lr == 0 || pr == 0 || er == 0) return false;
		cout  << "属性\t" << "等级\t" << "关卡\t" << "经验\t" << endl;
		cout << "排名\t" << lr << "\t" << pr << "\t" << er << "\t" << endl;
	}
	else
	{
		int lr = 0, wr = 0, i = 0;
		for (; i < dLevelRanking.size() && (lr == 0 || wr == 0); i++)
		{
			if (dLevelRanking[i] == n) lr = i + 1;
			if (wordRanking[i] == n) wr = i + 1;
		}
		if (lr == 0 || wr == 0) return false;
		cout <<"属性\t" << "等级\t" << "出题数\t" << endl;
		cout << "排名\t" << lr << "\t" << wr << "\t" << endl;
	}
	return true;
}

bool designer::showInfo()
{
	cout << "姓名\t" << "等级\t" << "出题数" << endl;
	cout << name << "\t" << level << "\t" << word << endl;
	cout << endl;

	rank(name, DESIGN);
	cout << "**************************************************************" << endl;
	return true;
}

void designer::addDesigner(string n)
{
	player::dLevelRanking.push_back(n);
	player::wordRanking.push_back(n);
}
