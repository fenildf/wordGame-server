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
extern std::map<std::string, player*> ChallengerInfo;//存储所有闯关者信息
extern std::map<std::string, player*> designerInfo;//存储所有出题者信息
extern std::map<int, std::vector<std::string>> vocabulary;//题目（单词）表
void clientLink(SOCKET ServerSocket, SOCKET clientSocket);