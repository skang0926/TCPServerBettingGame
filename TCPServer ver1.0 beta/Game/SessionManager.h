#ifndef _SESSIONMANAGER_H
#define _SESSIONMANAGER_H
#define _CRT_SECURE_NO_WARNINGS
#include "ClientSession.h"

class ClientSession;

struct Data
{
	char ID[16];
	char password[16];
};

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	ClientSession* CreateClient(SOCKET sock);
	void DeleteClient(ClientSession * client);

	int GetClientNumber(ClientSession* target);
	void LoadDataBase();

	void IncreaseSelectClientCount();

	int IncreaseClientCount();
	int DecreaseClientCount();

	int GetClientCount() const { return numOfClient; }
	int GetSelectClientCount() const { return selectClientCount; }

	bool SendBroadCast(PacketHeader* packet);
	bool Log(const char* fileName, const  char* id);

	int BSearchRecur(Data arr[], int first, int last, char* target);

	tm* GetTimeManager() const { return t; }
	Data* GetData() const { return data; }

	int GetDataBaseNum() const { return dataBaseNum; }

public:
	//static SessionManager* sessionManager;
	int Count;

private:
	CRITICAL_SECTION criticalSection;
	list<ClientSession*> clientlist;
	time_t timer;
	FILE *fp;
	tm *t;

	int selectClientCount;
	int numOfClient;

	Data* data;
	int dataBaseNum;

	char SaveTime[100];
	char SaveTime_hour[5];
	char SaveTime_min[5];
	char SaveTime_sec[5];
};

extern SessionManager* GSessionManager;

#endif