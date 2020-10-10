#include "PCH.h"

int main()
{
	TCPClient* client = new TCPClient;
	client->Initialize();
	client->Connect();
	client->Run();

	delete client;
	return 0;
}