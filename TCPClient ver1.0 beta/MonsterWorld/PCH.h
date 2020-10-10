#ifndef _PCH_H
#define _PCH_H 

#define BUFSIZE 512
#define MAXUSERNUM 3
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <process.h>

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

class GameObject;
class GameObjectManager;
class PacketHeader;



#include "Library.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Monster.h"
#include "Item.h"
#include "GameObjectManager.h"
#include "TCPClient.h"
#include "Packet.h"
#include "System.h"

#endif