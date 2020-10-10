#include "PCH.h"
#include "SessionManager.h"
#include "ClientSession.h"

short TCPServer::logNum = 0;

TCPServer::TCPServer() {}

TCPServer::~TCPServer() {}


bool TCPServer::Initialize(u_short port, const char* ip) {
	int retval;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {}

	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "failed Initialize()" << endl;
		return false;
	}

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);

	retval = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) {
		cout << "failed Initialize()" << endl;
		return false;
	}

	retval = listen(serverSocket, SOMAXCONN);
	if (retval == INVALID_SOCKET) {
		cout << "failed Initialize()" << endl;
		return false;
	}

	return true;
}

void TCPServer::CloseServer() {
	closesocket(serverSocket);
	WSACleanup();
}

bool TCPServer::Run()
{
	cout << "server is running..." << endl;

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET acceptSock;

	while (true) {

		acceptSock = accept(serverSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (acceptSock == INVALID_SOCKET)
		{
			cout << "accept error : Invalid socket..." << endl;
			break;
		}

		ClientSession* client = GSessionManager->CreateClient(acceptSock);
		if (client->OnConnect(clientAddr) == false)
		{
			cout << "connect error..." << endl;
			client->Disconnect();
			GSessionManager->DeleteClient(client);
		}
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, (LPVOID)client, 0, NULL);
		if (hThread == NULL)
		{
			cout << "Create thread error..." << endl;
			client->Disconnect();
			GSessionManager->DeleteClient(client);
		}
		else
		{
			CloseHandle(hThread);
		}
	}

	cout << "closed server..." << endl;

	return true;
}

unsigned int WINAPI TCPServer::WorkerThread(LPVOID lpParam)
{
	ClientSession * client = (ClientSession*)lpParam;
	char buf[BUFSIZE];
	PacketHeader* packet = (PacketHeader*)buf;

	while (client->IsConnected())
	{
		if (!client->Recv(packet))
		{
			Disconnect(packet, client);
			break;
		}
		cout << "received pakcet: " << packet->type << endl;

		switch (packet->type)
		{
		case PTC_USER_CON:
			Login(packet, client);
			break;

		case PTC_USER_INPUT:
			Input(packet, client);
			break;

		case PTC_USER_CHAT:
			Chat(packet, client);
			break;

		case PTC_USER_DISCON:
			cout << "logout" << endl;
			break;

		case PTC_USER_INFO:
			break;

		case PTC_USER_SELECT:
			break;

		case PTC_USER_WAIT:
			break;
		case PTC_USER_RENDER:
			Render(packet, client);
			break;
		case PTC_USER_END:
			GSessionManager->SendBroadCast(packet);
			break;
		}
	}

	return 0;
}



void TCPServer::Login(PacketHeader* packet, ClientSession * client)
{
	PacketConReceive* receivePacket = (PacketConReceive*)packet;

	char id[16];
	char password[16];

	int idx = GSessionManager->BSearchRecur(GSessionManager->GetData(),
		0, GSessionManager->GetDataBaseNum() - 1, receivePacket->userID);

	strcpy(id, GSessionManager->GetData()[idx].ID);
	strcpy(password, GSessionManager->GetData()[idx].password);

	if (strcmp(id, receivePacket->userID) == 0
		&& strcmp(password, receivePacket->userPassword) == 0)
	{
		GSessionManager->IncreaseClientCount();

		PacketUserInfo userInfo;

		userInfo.userNum = GSessionManager->GetClientCount();
		client->Send(&userInfo);
		client->SetClientID(id);
		client->SetLogined(true);

		PacketLog logPacket;
		time_t timer = time(NULL);

		strcpy(logPacket.ID, id);
		logPacket.t = *localtime(&timer);

		GSessionManager->SendBroadCast(&logPacket);

		PacketWait waitPacket;
		waitPacket.totalUser = GSessionManager->GetClientCount();

		GSessionManager->SendBroadCast(&waitPacket);
	}
	else
	{
		PacketConRequest requestPacket;
		client->Send(&requestPacket);
	}
}

void TCPServer::Input(PacketHeader* packet, ClientSession * client)
{
	GSessionManager->IncreaseSelectClientCount();
	if (GSessionManager->GetSelectClientCount() >= MAXUSERNUM)
	{		
		PacketRender RenderStart;
		GSessionManager->SendBroadCast(&RenderStart);
	}
}

void TCPServer::Render(PacketHeader* packet, ClientSession * client)
{
	GSessionManager->SendBroadCast(packet);
}

void TCPServer::Chat(PacketHeader* packet, ClientSession * client)
{
	cout << ((PacketChatReceive*)packet)->message << endl;
	GSessionManager->SendBroadCast(packet);
}

void TCPServer::Disconnect(PacketHeader* packet, ClientSession * client)
{
	PacketDisconnect disconPacket;
	disconPacket.userNum = GSessionManager->GetClientNumber(client);
	strcpy(disconPacket.ID, client->GetClientID());
	GSessionManager->SendBroadCast(&disconPacket);

	GSessionManager->DeleteClient(client);

	PacketWait waitPacket;
	waitPacket.totalUser = GSessionManager->GetClientCount();
	GSessionManager->SendBroadCast(&waitPacket);

	GSessionManager->Log("log", client->GetClientID());
}