#pragma once
#include <thread>
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <map>
#include <string>
#include <vector>
#include "player.h"
extern std::map<std::string, player*> ChallengerInfo;//�洢���д�������Ϣ
extern std::map<std::string, player*> designerInfo;//�洢���г�������Ϣ
extern std::map<int, std::vector<std::string>> vocabulary;//��Ŀ�����ʣ���
void clientLink(SOCKET ServerSocket, SOCKET clientSocket);