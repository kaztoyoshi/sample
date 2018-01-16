
#ifndef __NCCLIENT_INCLUDE__
#define __NCCLIENT_INCLUDE__

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT		0x0501												// WindowsXP以上

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "WL.h"
#include "resource.h"

#pragma comment( lib, "WS2_32.LIB" )

namespace	NetConfer
{

#define EXIT				1
#define TRUE				1
#define FALSE				0

#define TEXT_MAX			1024
#define PATH_MAX			4096
#define NAME_MAX			256

}

#endif
