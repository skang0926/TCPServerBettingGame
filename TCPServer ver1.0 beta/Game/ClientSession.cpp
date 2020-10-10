#include "PCH.h"
#include "ClientSession.h"

bool ClientSession::OnConnect(SOCKADDR_IN addr)
{
	//Ŭ���̾�Ʈ �ּ����� ȹ��.
	memcpy(&mClientAddr, &addr, sizeof(SOCKADDR_IN));
	int addrlen = sizeof(SOCKADDR_IN);
	getpeername(mSocket, (SOCKADDR *)&mClientAddr, &addrlen);
	//�������� ���� ����.
	mConnected = true;
	cout << "client Connected : IP = " << inet_ntoa(mClientAddr.sin_addr) << ",  Port = " << ntohs(mClientAddr.sin_port) << endl;
	//Ŭ���̾�Ʈ ���� ����..
	
	return true;
}

bool ClientSession::Send(PacketHeader* packet)
{
	if (!IsConnected()) return false;
	
	int retval;
	retval = send(mSocket, (char*)packet, packet->len, 0);
	if (retval == SOCKET_ERROR) {
		return false;
	}

	return true;
}

bool ClientSession::Recv(PacketHeader* packet)
{
	if (!IsConnected()) return false;
	
	int retval;	
	retval = recv(mSocket, (char*)packet, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		return false;
	}

	return true;
}

void ClientSession::Disconnect()
{
	cout << "client disconnected IP = " << inet_ntoa(mClientAddr.sin_addr) << ",  Port = " << ntohs(mClientAddr.sin_port) << endl;
	//���� �Ŵ������� �� ����
	if (mLogined == true)
	{
		cout << "true" << endl;
		GSessionManager->DecreaseClientCount();
	}
	
	//���� ����
	closesocket(mSocket);
	//���Ӳ������� ���� ����.
	mConnected = false;
}
