#include "ClientThread.h"
#include "game.h"
#include <vector>
#include <string>

void clientLink(SOCKET *ServerSocket, SOCKET *clientSocket)
{
	game g;
	g.run();
}