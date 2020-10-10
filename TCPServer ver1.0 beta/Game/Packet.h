#include "PCH.h"

enum PacketType {
	PTC_USER_CON = 100,
	PTC_USER_WAIT,
	PTC_USER_INFO,
	PTC_USER_INPUT,
	PTC_USER_SELECT,
	PTC_USER_DISCON,	
	PTC_USER_RENDER,
	PTC_USER_CHAT,
	PTC_USER_END,
	PTC_USER_LOG,
	PTC_NONE
};

enum UserInputType
{
	USER_INPUT_ONE = 1, USER_INPUT_TWO, USER_INPUT_THREE, USER_INPUT_FOUR, USER_INPUT_FIVE
};

class PacketHeader
{
public:
	PacketHeader() : type(PTC_NONE), len(0) {}

	PacketHeader(PacketType type) : type(type), len(0) {}

	enum PacketType type;
	short			len;
};

class PacketConRequest : public PacketHeader
{
public:
	PacketConRequest() : PacketHeader(PTC_USER_CON)
	{
		len = sizeof(PacketConRequest);
	}

	short userNum;
};

class PacketConReceive : public PacketHeader
{
public:
	PacketConReceive() : PacketHeader(PTC_USER_CON)
	{
		len = sizeof(PacketConReceive);
	}

	char userID[16];
	char userPassword[16];
};

class PacketWait : public PacketHeader
{
public:
	PacketWait() : PacketHeader(PTC_USER_WAIT)
	{
		len = sizeof(PacketWait);
	}

	short totalUser;
};

class PacketLog: public PacketHeader
{
public:
	PacketLog() : PacketHeader(PTC_USER_LOG)
	{
		len = sizeof(PacketLog);
	}

	char ID[16];

	tm t;
};

class PacketUserInfo : public PacketHeader
{
public:
	PacketUserInfo() : PacketHeader(PTC_USER_INFO)
	{
		len = sizeof(PacketUserInfo);
	}

	short userNum;
};

class PacketInputRequest : public PacketHeader
{
public:
	PacketInputRequest() : PacketHeader(PTC_USER_INPUT)
	{
		len = sizeof(PacketInputRequest);
	}

	enum UserInputType inputType = USER_INPUT_ONE;

};

class PacketInputReceive : public PacketHeader
{
public:
	PacketInputReceive() : PacketHeader(PTC_USER_INPUT)
	{
		len = sizeof(PacketInputReceive);
	}

	enum UserInputType inputType = USER_INPUT_ONE;

};

class PacketDisconnect : public PacketHeader
{
public:
	PacketDisconnect() : PacketHeader(PTC_USER_DISCON)
	{
		len = sizeof(PacketDisconnect);
	}

	char ID[16];
	short userNum;
};

class PacketChatRequest : public PacketHeader
{
public:
	PacketChatRequest() : PacketHeader(PTC_USER_CHAT), message("")
	{
		len = sizeof(PacketChatRequest);
	}

	char message[48 + 1];
};

class PacketChatReceive : public PacketHeader
{
public:
	PacketChatReceive() : PacketHeader(PTC_USER_CHAT), message("")
	{
		len = sizeof(PacketChatReceive);
	}

	char message[48 + 1];
};

class PacketRender : public PacketHeader
{
public:
	PacketRender() : PacketHeader(PTC_USER_RENDER), screen(""), counts()
	{
		len = sizeof(PacketRender);
	}

	char screen[301];	
	int counts[5];
	
};

class PacketEnd : public PacketHeader
{
public:
	PacketEnd() : PacketHeader(PTC_USER_END), inputType(0), winType()
	{
		len = sizeof(PacketEnd);
	}

	int inputType;
	int winType[5];
};