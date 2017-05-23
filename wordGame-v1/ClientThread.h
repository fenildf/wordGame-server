#pragma once
#include <thread>
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <WS2tcpip.h>
void clientLink(SOCKET *ServerSocket, SOCKET *clientSocket);