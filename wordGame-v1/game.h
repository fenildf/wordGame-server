#pragma once
#include "player.h"
#include <map>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX_BUFLEN 512
enum OPTION{LOGIN,REGISTER,LOGOUT,QUIT};

//游戏系统类，实现启动、用户登录、注册、游戏等功能
class game
{
public:
	game(SOCKET sSocket = 0, SOCKET cSocket = 0);
	~game();
	void run();//运行游戏
private:
	Mode Login();//玩家登录
	Mode Register();//玩家注册
	int userIn();//玩家选择登录，注册或退出，第一级界面
	int challengeUi();//闯关者界面
	void challenge();//闯关者进行游戏
	void rank();//查看排名
	void search();//查找玩家
	void searchChallenger(std::string name, int level, int exp, int pass);//查找闯关者
	void searchDesigner(std::string name, int level, int word);//查找出题者
	int designUi();//出题者界面
	void design();//出题者出题
	

	//加载闯关者，出题者和题目数据
	void loadChallenger();
	void loadDesigner();
	void loadVocabulary();
	//保存闯关者，出题者和题目数据
	void saveChallenger();
	void saveDesigner();
	void saveVocabulary();

	player *currentPlayer;//当前用户指针
	SOCKET serverSocket;
	SOCKET clientSocket;
	int currentPass;//当前关卡
	Mode mode;//当前登录模式
	std::map<std::string, player*> ChallengerInfo;//存储所有闯关者信息
	std::map<std::string, player*> designerInfo;//存储所有出题者信息
	std::map<int, std::vector<std::string>> vocabulary;//题目（单词）表
	char recvbuf[MAX_BUFLEN];
};

