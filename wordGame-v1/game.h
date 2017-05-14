#pragma once
#include "player.h"
#include <map>
#include <vector>


enum OPTION{LOGIN,REGISTER,LOGOUT,QUIT};
//��Ϸϵͳ�࣬ʵ���������û���¼��ע�ᡢ��Ϸ�ȹ���
class game
{
public:
	game();
	~game();
	void run();//������Ϸ
private:
	Mode Login();//�û���¼
	Mode Register();//�û�ע��
	int userIn();
	int challengeUi();//�����ߴ���
	void rank();
	void search();
	void searchChallenger(string name, int level, int exp, int pass);
	void searchDesigner(string name, int level, int word);
	int designUi();//�����߳���
	

	//��������
	void loadChallenger();
	void loadDesigner();
	//��������
	void saveChallenger();
	void saveDesigner();

	player *currentPlayer;//��ǰ�û�ָ��
	int currentPass;//��ǰ�ؿ�
	Mode mode;//��ǰ��¼ģʽ
	map<string, player*> ChallengerInfo;//�洢���д�������Ϣ
	map<string, player*> designerInfo;
};

