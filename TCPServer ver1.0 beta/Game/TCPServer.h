#include "ClientSession.h"

class TCPServer 
{
public:
	TCPServer();
	~TCPServer();

	bool Initialize(u_short port = 9000, const char* ip = "127.0.0.1");
	void CloseServer();
	bool Run();

	SOCKET GetSocket() const { return serverSocket; }

private:
	static unsigned int WINAPI WorkerThread(LPVOID lpParam);

	static void Login(PacketHeader* packet, ClientSession * client);
	static void Input(PacketHeader* packet, ClientSession * client);
	static void Render(PacketHeader* packet, ClientSession * client);
	static void Chat(PacketHeader* packet, ClientSession * client);
	static void Disconnect(PacketHeader* packet, ClientSession * client);

	static short logNum;
private:
	SOCKET serverSocket;
};