#include "ClientThread.h"
#include "game.h"
#include <vector>
#include <string>
#include <fstream>
std::map<std::string, player*> ChallengerInfo;//存储所有闯关者信息
std::map<std::string, player*> designerInfo;//存储所有出题者信息
std::map<int, std::vector<std::string>> vocabulary;//题目（单词）表

void saveChallenger()
{
	std::ofstream out("challenger.txt");
	auto begin = ChallengerInfo.begin(), end = ChallengerInfo.end();

	while (begin != end)
	{
		out << begin->second->getName() << " " << begin->second->getPw() << " " << begin->second->getLevel() << " "
			<< begin->second->getPass() << " " << begin->second->getExp() << std::endl;
		begin++;
	}
	out.close();
}

void saveDesigner()
{
	std::ofstream out("designer.txt");
	auto begin = designerInfo.begin(), end = designerInfo.end();

	while (begin != end)
	{
		out << begin->second->getName() << " " << begin->second->getPw() << " " << begin->second->getLevel() << " "
			<< begin->second->getPuzzle() << std::endl;
		begin++;
	}
	out.close();
}

void saveVocabulary()
{
	std::ofstream out("vocabulary.txt");
	auto begin = vocabulary.begin(), end = vocabulary.end();
	while (begin != end)
	{
		for (auto word : begin->second)
		{
			out << word << std::endl;
		}
		begin++;
	}
	out.close();
}

void clientLink(SOCKET ServerSocket, SOCKET clientSocket)
{
	game g(&ChallengerInfo, &designerInfo, &vocabulary, ServerSocket, clientSocket);
	g.run();
	saveChallenger();
	saveDesigner();
	saveVocabulary();
}