#include "PCH.h"
#include "TCPClient.h"



TCPClient::TCPClient() : objManager(5)
{

}

TCPClient::~TCPClient()
{
}

void TCPClient::Initialize(u_short port, const char* ip)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = inet_addr(ip);
}

void TCPClient::Connect()
{
	int retval;
	retval = connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	if (retval) {
		cout << "connect() failed" << endl;
	}
}

bool TCPClient::Send(PacketHeader* packet)
{
	int retval;
	retval = send(sock, (char*)packet, packet->len, 0);
	if (retval == SOCKET_ERROR) {
		return false;
	}

	return true;
}

bool TCPClient::Receive(PacketHeader* packet)
{
	int retval;
	retval = recv(sock, (char*)packet, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		return false;
	}

	return true;
}

void TCPClient::ShowLog(PacketHeader* packet)
{
	cout << endl << endl << ((PacketLog*)packet)->t.tm_year + 1900 << "년도 ";
	cout << ((PacketLog*)packet)->t.tm_mon + 1 << "월 ";
	cout << ((PacketLog*)packet)->t.tm_mday << "일 ";
	cout << ((PacketLog*)packet)->t.tm_hour << "시 ";
	cout << ((PacketLog*)packet)->t.tm_min << "분 ";
	cout << ((PacketLog*)packet)->t.tm_sec << "초 ";
	cout << ((PacketLog*)packet)->ID << " connected";
}

void TCPClient::Run()
{
	PacketHeader* packet = (PacketHeader*)buf;

	if (!Login()) return;
	while (true)
	{
		if (!Receive(packet)) break;

		switch (packet->type)
		{
		case PTC_USER_CON:
			Login();
			break;
		case PTC_USER_INPUT:

			break;

		case PTC_USER_CHAT:
			Chat(packet);
			break;

		case PTC_USER_DISCON:
			Disconnect(packet);
			break;

		case PTC_USER_INFO:
			clientNumber = ((PacketUserInfo*)packet)->userNum;
			break;

		case PTC_USER_SELECT:
			break;

		case PTC_USER_WAIT:
			Wait(packet);
			break;

		case PTC_USER_RENDER:
			Render(packet);
			break;

		case PTC_USER_LOG:
			ShowLog(packet);
			break;

		case PTC_USER_END:
			End(packet);
			break;
		}
	}
}

bool TCPClient::Login()
{
	system("cls");
	char id[16], password[16];

	cout << "ID: ";
	cin >> id;
	cout << "Password: ";
	cin.ignore();

	int cnt = 0;
	while (1)
	{
		password[cnt] = getch();

		if (password[cnt++] == 13)
		{
			password[cnt - 1] = '\0';
			break;
		}
		else
			cout << '*';
	}

	PacketConRequest request;
	strcpy(request.userID, id);
	strcpy(request.userPassword, password);
	Send(&request);

	return true;
}


void TCPClient::Wait(PacketHeader* packet)
{
	Borland::Gotoxy(0, 0);
	cout << "My number: " << clientNumber << endl;
	cout << "플레이어 접속 대기중." << endl;
	cout << "현재 플레이어 수: " << ((PacketWait*)packet)->totalUser << endl;

	if (((PacketWait*)packet)->totalUser >= MAXUSERNUM)
	{
		Input();
		//_beginthreadex(NULL, 0, Chat, (LPVOID)this, 0, NULL);
	}
}

void TCPClient::Input()
{
	system("cls");
	PacketInputRequest inputRequest;

	int input;
	cout << clientNumber << endl;
	cout << "*1번에서 5번 말을 선택: ";

	cin >> input;

	switch (input)
	{
	case 1:
		inputRequest.inputType = USER_INPUT_ONE;
		break;

	case 2:
		inputRequest.inputType = USER_INPUT_TWO;
		break;

	case 3:
		inputRequest.inputType = USER_INPUT_THREE;
		break;

	case 4:
		inputRequest.inputType = USER_INPUT_FOUR;
		break;

	case 5:
		inputRequest.inputType = USER_INPUT_FIVE;
		break;
	}

	select = input;

	Send(&inputRequest);
}

void TCPClient::Render(PacketHeader* packet)
{
	PacketRender* renderPacket = (PacketRender*)packet;
	Renderer& renderer = Renderer::Instance();

	if (clientNumber == 1)
	{
		renderer.Clear();
		objManager.Update();

		PacketRender sendPacket;

		string& map = renderer.GetScreen();
		strcpy(sendPacket.screen, map.c_str());

		vector<int> counts = objManager.GetCounts();
		for (int i = 0; i < counts.size(); ++i) {
			sendPacket.counts[i] = counts[i];
		}

		if (objManager.GetNumOfItems() == 0)
		{
			PacketEnd endPacket;
			int max = counts[0];
			for (int i = 0; i < counts.size(); ++i) {
				if (max < counts[i]) {
					max = counts[i];
				}
			}

			for (int i = 0, j = 0; i < counts.size(); ++i) {
				if (max == counts[i]) {
					endPacket.winType[j++] = i + 1;
				}
			}

			Send(&endPacket);
			return;
		}

		Send(&sendPacket);
	}
	else {
		renderer.SetScreen(renderPacket->screen);
		objManager.SetCounts(renderPacket->counts);
	}

	renderer.Render();
	renderer.ShowStatus(objManager);
}

void TCPClient::Chat(PacketHeader *packet)
{
	static int line = 0;
	PacketChatReceive* receivePacket = (PacketChatReceive*)packet;
	Borland::Gotoxy(50, 15 + line);
	line++;
	cout << receivePacket->message << endl;

	Renderer& renderer = Renderer::Instance();
	PacketRender sendPacket;
	string& map = renderer.GetScreen();
	strcpy(sendPacket.screen, map.c_str());
	vector<int> counts = objManager.GetCounts();
	for (int i = 0; i < counts.size(); ++i) {
		sendPacket.counts[i] = counts[i];
	}
	Send(&sendPacket);
}

void TCPClient::End(PacketHeader* packet)
{
	Renderer& renderer = Renderer::Instance();
	renderer.GameOver();

	PacketEnd* endPacket = (PacketEnd*)packet;

	for (int i = 0; i < 5; ++i)
	{
		if (endPacket->winType[i] == 0) continue;

		if (select == endPacket->winType[i])
		{
			cout << "win" << endl;
			return;
		}
	}
	cout << "lose" << endl;
	system("pause");
}

void TCPClient::Disconnect(PacketHeader* packet)
{
	PacketDisconnect* disconPacket = (PacketDisconnect*)packet;

	cout << endl << endl << endl << endl << "ID: " << disconPacket->ID << "가 종료 되었습니다." << endl;

	if (disconPacket->userNum < clientNumber) --clientNumber;
}

unsigned int WINAPI TCPClient::Chat(LPVOID lpParam)
{
	TCPClient* client = (TCPClient*)lpParam;

	while (true)
	{
		//if (System::GetKeyDown(KeyCode::Enter)) {
		//	PacketChatRequest messagePacket;
		//	string& str = System::GetStr();
		//	strcpy(messagePacket.message, str.c_str());
		//	client->Send(&messagePacket);
		//	System::Clear();
		//	System::EndOfFrame();
		//}
	}

}