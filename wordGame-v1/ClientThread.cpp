#include "ClientThread.h"
#include "game.h"
#include <vector>
#include <string>
std::map<std::string, player*> ChallengerInfo;//存储所有闯关者信息
std::map<std::string, player*> designerInfo;//存储所有出题者信息
std::map<int, std::vector<std::string>> vocabulary;//题目（单词）表
void clientLink(SOCKET ServerSocket, SOCKET clientSocket)
{
	game g(&ChallengerInfo,&designerInfo,&vocabulary, ServerSocket, clientSocket);
	g.run();
}
