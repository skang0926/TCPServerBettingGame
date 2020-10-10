#include "PCH.h"
#include "ClientSession.h"

bool ClientSession::OnConnect(SOCKADDR_IN addr)
{
	//클라이언트 주소정보 획득.
	memcpy(&mClientAddr, &addr, sizeof(SOCKADDR_IN));
	int addrlen = sizeof(SOCKADDR_IN);
	getpeername(mSocket, (SOCKADDR *)&mClientAddr, &addrlen);
	//접속으로 상태 변경.
	mConnected = true;
	cout << "client Connected : IP = " << inet_ntoa(mClientAddr.sin_addr) << ",  Port = " << ntohs(mClientAddr.sin_port) << endl;
	//클라이언트 개수 증가..
	
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
	//세션 매니저에서 수 감소
	if (mLogined == true)
	{
		cout << "true" << endl;
		GSessionManager->DecreaseClientCount();
	}
	
	//소켓 종료
	closesocket(mSocket);
	//접속끊김으로 상태 변경.
	mConnected = false;
}
