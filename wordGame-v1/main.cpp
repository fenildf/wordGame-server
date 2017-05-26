//#define WIN32_LEAN_AND_MEAN

#include "ClientThread.h"
#include <thread>
#include <Windows.h>
#include<WinSock2.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "game.h"
#include "player.h"
#pragma comment (lib,"ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void loadChallenger(std::map<std::string, player*> &ChallengerInfo)
{
	std::ifstream in("challenger.txt");
	std::string name, password;
	int level, pass, exp;
	while (in >> name)
	{
		in >> password >> level >> pass >> exp;
		ChallengerInfo[name] = new challenger(name, password, level, pass, exp);
	}
	in.close();
}

void loadDesigner(std::map<std::string, player*> &designerInfo)
{
	std::ifstream in("designer.txt");
	std::string name, password;
	int level, word;
	while (in >> name)
	{
		in >> password >> level >> word;
		designerInfo[name] = new designer(name, password, level, word);
	}
	in.close();
}

void loadVocabulary(std::map<int, std::vector<std::string>> &vocabulary)
{
	std::ifstream in("vocabulary.txt");
	std::string word;
	int l = 0;
	while (in >> word)
	{
		if (word.size() > l) l = word.size();
		vocabulary[l].push_back(word);
	}
}

void saveChallenger(std::map<std::string, player*> &ChallengerInfo)
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

void saveDesigner(std::map<std::string, player*> &designerInfo)
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

void saveVocabulary(std::map<int, std::vector<std::string>> &vocabulary)
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

int main()
{
	player::allRankInit();
	challenger::levelExpInit();
	designer::levelPuzzleInit();
	
	loadChallenger(ChallengerInfo);
	loadDesigner(designerInfo);
	loadVocabulary(vocabulary);
	
	//联网功能初始化
	WSADATA wsaData;
	int iResult;
	
	SOCKET listenSocket = INVALID_SOCKET, clientSocket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo hints;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "联网服务启动失败！" << std::endl;
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result) != 0)
	{
		std::cout << "联网失败！" << std::endl;
		WSACleanup();
		return 1;
	}
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "联网失败!" << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		std::cout << "联网失败！" << std::endl;
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "联网失败！" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	
	std::vector<std::thread *> multiClient;
	
	//开始等待连接
	while (true)
	{
		clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) continue;
		multiClient.push_back(new std::thread(clientLink, listenSocket, clientSocket));
	}

	system("pause");
	return 0;
}

