#include "PCH.h"
int main(void) {
	bool retval;
	TCPServer *server = new TCPServer;

	retval = server->Initialize();
	if (!retval) return -1;

	retval = server->Run();
	if (!retval) return -1;

	server->CloseServer();
	delete server;

	return 0;
}