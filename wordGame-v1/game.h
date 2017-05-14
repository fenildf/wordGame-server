#pragma once
#include "player.h"
#include <map>
#include <vector>


enum OPTION{LOGIN,REGISTER,LOGOUT,QUIT};
//游戏系统类，实现启动、用户登录、注册、游戏等功能
class game
{
public:
	game();
	~game();
	void run();//运行游戏
private:
	Mode Login();//用户登录
	Mode Register();//用户注册
	int userIn();
	int challengeUi();//闯关者闯关
	void rank();
	void search();
	void searchChallenger(string name, int level, int exp, int pass);
	void searchDesigner(string name, int level, int word);
	int designUi();//出题者出题
	

	//加载数据
	void loadChallenger();
	void loadDesigner();
	//保存数据
	void saveChallenger();
	void saveDesigner();

	player *currentPlayer;//当前用户指针
	int currentPass;//当前关卡
	Mode mode;//当前登录模式
	map<string, player*> ChallengerInfo;//存储所有闯关者信息
	map<string, player*> designerInfo;
};

