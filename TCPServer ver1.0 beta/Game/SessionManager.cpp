#include "PCH.h"
#include "SessionManager.h"

SessionManager* GSessionManager = new SessionManager;

SessionManager::SessionManager() 
{
	LoadDataBase();

	selectClientCount = 0;
	numOfClient = 0;
	InitializeCriticalSection(&criticalSection);
}

SessionManager::~SessionManager() {
	DeleteCriticalSection(&criticalSection);
	if (data)
	{
		delete[] data;
	}
}

ClientSession * SessionManager::CreateClient(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);
	cout << "create ClientSession.." << endl;
	//클라이언트를 리스트에 추가.
	clientlist.push_back(client);
	return client;
}

void SessionManager::DeleteClient(ClientSession * client)
{
	//iterator를 통해 클라이언트 삭제.
	list<ClientSession*>::iterator iter;
	for (iter = clientlist.begin(); iter != clientlist.end(); ) {
		if (*iter == client) {
			//클라 접속 종료
			client->Disconnect();
			//리스트에서 삭제.
			iter = clientlist.erase(iter);
			cout << "client remove.." << endl;
		}
		else {
			iter++;
		}
	}
}

int SessionManager::GetClientNumber(ClientSession* target)
{
	int index = 0;
	for (auto client : clientlist) {
		if (client == target) return index;
		++index;
	}

	return -1;
}

void SessionManager::IncreaseSelectClientCount()
{

	selectClientCount++;

}

int SessionManager::IncreaseClientCount()
{
	//임계영역 진입
	EnterCriticalSection(&criticalSection);
	//클라이언트 개수 증가.
	numOfClient++;
	cout << "client count increase.." << endl;
	//임계영역 탈출
	LeaveCriticalSection(&criticalSection);

	return numOfClient;
}

int SessionManager::DecreaseClientCount()
{
	EnterCriticalSection(&criticalSection);
	numOfClient--;
	cout << "client count decrease.." << endl;
	LeaveCriticalSection(&criticalSection);

	return numOfClient;
}

bool SessionManager::SendBroadCast(PacketHeader* packet)
{
	for (auto client : clientlist)
	{
		client->Send(packet);
	}

	return true;
}

int SessionManager::BSearchRecur(Data arr[], int first, int last, char* target)
{
	int mid = (first + last) / 2;
	
	if (first > last) return -1;
	else
	{
		if (strcmp(arr[mid].ID, target) == 0)
		{		
			return mid;
		}
		else
		{
			if (strcmp(arr[mid].ID, target) > 0)
			{
				last = mid - 1;
				BSearchRecur(arr, first, last, target);
			}
			else
			{
				first = mid + 1;
				BSearchRecur(arr, first, last, target);
			}
		}
	}
}

void SessionManager::LoadDataBase()
{
	char input;
	ifstream fin;

	fin.open("UserData.dot");
	if (fin.fail()) return;

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin >> dataBaseNum;

	data = new Data[dataBaseNum];

	for (int i = 0; i < dataBaseNum; i++)
	{
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin >> data[i].ID;

		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin >> data[i].password;
	}

	fin.close();
}

bool SessionManager::Log(const char* name, const char* id)
{
	string fileName = name;
	fileName += ".dot";

	timer = time(NULL);
	t = localtime(&timer);

	_itoa(t->tm_hour, SaveTime_hour, 10);
	_itoa(t->tm_min, SaveTime_min, 10);
	_itoa(t->tm_sec, SaveTime_sec, 10);

	fp = fopen(fileName.c_str(), "a+");

	if (!fp) return false;
	fprintf(fp, "%s:%s:%s:%s 종료\n", id, SaveTime_hour, SaveTime_min, SaveTime_sec);
	fclose(fp);

	return true;
}