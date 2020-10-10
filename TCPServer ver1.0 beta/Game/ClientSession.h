#ifndef _CLIENTSESSION_H
#define _CLIENTSESSION_H
#include "SessionManager.h"

class SessionManager;

class ClientSession
{
public:
	ClientSession(SOCKET sock) : mSocket(sock), mConnected(false)
	{
		//주소 초기화.
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	}
	~ClientSession() {}

	int GetSocket() const { return mSocket; }

	void	SetClientID(char* id) { strcpy(mID, id); };
	char*	GetClientID() { return mID;}
	void	SetLogined(bool value) { mLogined = value; }

	//연결확인 함수.
	bool	OnConnect(SOCKADDR_IN addr);
	bool	IsConnected() const { return mConnected; }

	bool Send(PacketHeader* packet);
	bool Recv(PacketHeader* packet);

	//연결 종료 함수.
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