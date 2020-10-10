#ifndef _CLIENTSESSION_H
#define _CLIENTSESSION_H
#include "SessionManager.h"

class SessionManager;

class ClientSession
{
public:
	ClientSession(SOCKET sock) : mSocket(sock), mConnected(false)
	{
		//�ּ� �ʱ�ȭ.
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	}
	~ClientSession() {}

	int GetSocket() const { return mSocket; }

	void	SetClientID(char* id) { strcpy(mID, id); };
	char*	GetClientID() { return mID;}
	void	SetLogined(bool value) { mLogined = value; }

	//����Ȯ�� �Լ�.
	bool	OnConnect(SOCKADDR_IN addr);
	bool	IsConnected() const { return mConnected; }

	bool Send(PacketHeader* packet);
	bool Recv(PacketHeader* packet);

	//���� ���� �Լ�.
	void Disconnect();

private:
	SOCKET			mSocket;
	SOCKADDR_IN		mClientAddr;
	bool			mConnected;
	bool			mLogined;
	char         	mID[16];

	friend class SessionManager;
};
#endif