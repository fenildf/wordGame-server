#pragma once
#include "player.h"
#include <map>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX_BUFLEN 512
enum OPTION{LOGIN,REGISTER,LOGOUT,QUIT};

//��Ϸϵͳ�࣬ʵ���������û���¼��ע�ᡢ��Ϸ�ȹ���
class game
{
public:
	game(SOCKET sSocket = 0, SOCKET cSocket = 0);
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
	void searchChallenger(std::string name, int level, int exp, int pass);//���Ҵ�����
	void searchDesigner(std::string name, int level, int word);//���ҳ�����
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
	SOCKET serverSocket;
	SOCKET clientSocket;
	int currentPass;//��ǰ�ؿ�
	Mode mode;//��ǰ��¼ģʽ
	std::map<std::string, player*> ChallengerInfo;//�洢���д�������Ϣ
	std::map<std::string, player*> designerInfo;//�洢���г�������Ϣ
	std::map<int, std::vector<std::string>> vocabulary;//��Ŀ�����ʣ���
	char recvbuf[MAX_BUFLEN];
};

