#define WIN32_LEAN_AND_MEAN

#include "ClientThread.h"
#include <thread>
#include <Windows.h>
#include<WinSock2.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "game.h"
#include "player.h"
#pragma comment (lib,"ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int main()
{
	/*game g;
	g.run();*/
	player::allRankInit();
	challenger::levelExpInit();
	designer::levelPuzzleInit();

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