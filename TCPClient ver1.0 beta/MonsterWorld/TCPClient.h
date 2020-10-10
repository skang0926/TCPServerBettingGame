#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H



class TCPClient
{
public:
	TCPClient();
	~TCPClient();

	void Initialize(u_short port = 9000, const char* ip = "127.0.0.1");
	void Connect();
	void Run();

private:	
	static unsigned int WINAPI Chat(LPVOID lpParam);
	
	bool Send(PacketHeader* packet);
	bool Receive(PacketHeader* packet);

	void Input();
	void Render(PacketHeader *packet);
	void Chat(PacketHeader *packet);
	bool Login();
	void Wait(PacketHeader* packet);
	void ShowLog(PacketHeader* packet);
	void End(PacketHeader* packet);
	void Disconnect(PacketHeader* packet);
	
private:
	
	

	GameObjectManager objManager;
	SOCKET sock;
	SOCKADDR_IN servAddr;
	char buf[BUFSIZE];
	int clientNumber;
	int select;
};
#endif