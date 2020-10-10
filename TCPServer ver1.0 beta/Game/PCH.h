#ifndef _PCH_H
#define _PCH_H

#define MAXUSERNUM 3
#define BUFSIZE	   512
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <stdlib.h>
#include <WinSock2.h>
#include <process.h>
#include <time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

#include "Packet.h"
#include "TCPServer.h"

#endif