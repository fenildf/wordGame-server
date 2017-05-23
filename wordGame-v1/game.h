#pragma once
#include "player.h"
#include <map>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

enum OPTION{LOGIN,REGISTER,LOGOUT,QUIT};

//��Ϸϵͳ�࣬ʵ���������û���¼��ע�ᡢ��Ϸ�ȹ���
class game
{
public:
	game(SOCKET *sSocket = nullptr, SOCKET *cSocket = nullptr);
	~game();
	void run();//������Ϸ
private:
	Mode Login();//��ҵ�¼
	Mode Register();//���ע��
	int userIn();//���ѡ���¼��ע����˳�����һ������
	int challengeUi();//�����߽���
	void challenge();//�����߽�����Ϸ
	void rank();//�鿴����
	void search();//�������
	void searchChallenger(string name, int level, int exp, int pass);//���Ҵ�����
	void searchDesigner(string name, int level, int word);//���ҳ�����
	int designUi();//�����߽���
	void design();//�����߳���
	

	//���ش����ߣ������ߺ���Ŀ����
	void loadChallenger();
	void loadDesigner();
	void loadVocabulary();
	//���洳���ߣ������ߺ���Ŀ����
	void saveChallenger();
	void saveDesigner();
	void saveVocabulary();

	player *currentPlayer;//��ǰ�û�ָ��
	SOCKET *serverSocket;
	SOCKET *clientSocket;
	int currentPass;//��ǰ�ؿ�
	Mode mode;//��ǰ��¼ģʽ
	map<string, player*> ChallengerInfo;//�洢���д�������Ϣ
	map<string, player*> designerInfo;//�洢���г�������Ϣ
	map<int, vector<string>> vocabulary;//��Ŀ�����ʣ���
};

