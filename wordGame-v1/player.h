#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int EXP_PER_PASS = 100;
const int MAX_LEVEL = 100;
enum Mode { CHALLENGE, DESIGN, FAIL };
enum ATTR {LEVEL,PASS,EXP,PUZZLE};
//闯关者和出题者共有的抽象基类
class player {
public:
	player():level(0),clgInfo(nullptr),dsInfo(nullptr){}
	player(const player& p);
	player& operator=(const player& p);
	player(std::string n, std::string pw, int l = 0, int ol = 0) :name(n), password(pw), level(l), clgInfo(nullptr), dsInfo(nullptr), online(ol)
	{
		busy = 0;
		isNew = 0;
		chalOk = 0;
		wasCha = 0;
		reply = 0;
		over = 1;
	}
	virtual ~player()
	{

	}

	int getLevel() const { return level; }
	std::string getName () const { return name; }
	std::string getPw() const { return password; }
	virtual int getPass() const { return 0; }
	virtual int getExp() const { return 0; }
	virtual int getPuzzle() const { return 0; }
	void getAllUser(std::map<std::string, player*> *pclg, std::map<std::string, player*> *pds) { clgInfo = pclg, dsInfo = pds; }
	virtual bool rank(std::string n,Mode m) = 0;
	virtual bool showInfo(char recvbuf[]) = 0;
	//virtual void levelInc(int exp) = 0;
	virtual void reRank() = 0;
	virtual void refreshInfo(int d) = 0;
	virtual int getAttr(ATTR a);
	virtual bool solve(std::string puzzle) { return true; }
	virtual std::string design(std::vector<std::string> &vocabulary) { return std::string(); }
	static void allRankInit();
	static void saveAllRank();
	void setSocket(SOCKET cSocket) { currentSocket = cSocket; }
	int isOnline() { return online; }
	void setOnline() { online = 1; }
	int busy;
	int isNew;
	int chalOk;
	int wasCha;
	int reply;
	int over;
	//pair<int, int> result;
	int judge;
	SOCKET currentSocket;
protected:
	std::string name;
	std::string password;
	void attrReRank(std::vector<std::string> &ranking,int attr,Mode m,ATTR a);
	std::map<std::string, player*> *clgInfo;
	std::map<std::string, player*> *dsInfo;
	int level;
	
	int online;
	static std::vector<std::string> cLevelRanking;
	static std::vector<std::string> dLevelRanking;
	static std::vector<std::string> expRanking;
	static std::vector<std::string> passRanking;
	static std::vector<std::string> puzzleRanking;
	static void rankInit(std::vector<std::string> &ranking, std::string file);
	static void saveRank(std::vector<std::string> ranking, std::string file);
};

class challenger:public player
{
public:
	challenger():pass(0), exp(0) {}
	challenger(const challenger &c);
	challenger& operator=(challenger &c);
	challenger(std::string n, std::string pw, int l=0,int p=0,int e=0):pass(p),exp(e),player(n,pw,l){}
	virtual ~challenger();

	int getPass() const { return pass; }
	int getExp() const { return exp; }
	bool solve(std::string puzzle);
	bool rank(std::string n,Mode m);
	bool showInfo(char recvbuf[]);
	void reRank();
	//void levelInc(int dExp);
	void refreshInfo(int d);
	static void addChallenger(std::string n);
	static void levelExpInit();
private:
	
	int exp;
	int pass;

	static std::vector<int> levelExp;
	
};


class designer : public player
{
public:
	designer();
	designer(const designer &d);
	designer& operator=(const designer &d);
	designer(std::string n, std::string pw, int l = 0, int w = 0) :puzzle(w), player(n, pw, l) {  }
	~designer();
	static void levelPuzzleInit();
	std::string design(std::vector<std::string> &vocabulary) { return std::string(); }
	bool rank(std::string n, Mode m);

	void refreshInfo(int d);
	bool showInfo(char recvbuf[]);
	void reRank();

	int getPuzzle() const { return puzzle; }
	static void addDesigner(std::string n);
private:
	int puzzle;
	static std::vector<int> levelPuzzle;
	
};

inline
designer::designer()
{
}

inline designer::designer(const designer & d):player(d),puzzle(d.puzzle)
{

}

inline designer & designer::operator=(const designer & d)
{
	player::operator=(d);
	puzzle = d.puzzle;
	return *this;
}

inline
designer::~designer()
{
}

inline void designer::levelPuzzleInit()
{
	levelPuzzle.push_back(0);
	for (int i = 1; i < 100; i++)
	{
		levelPuzzle.push_back(i+levelPuzzle[i-1]);
	}
}
