#include "ClientThread.h"
#include "game.h"
#include <vector>
#include <string>
std::map<std::string, player*> ChallengerInfo;//�洢���д�������Ϣ
std::map<std::string, player*> designerInfo;//�洢���г�������Ϣ
std::map<int, std::vector<std::string>> vocabulary;//��Ŀ�����ʣ���
void clientLink(SOCKET ServerSocket, SOCKET clientSocket)
{
	game g(&ChallengerInfo,&designerInfo,&vocabulary, ServerSocket, clientSocket);
	g.run();
}
