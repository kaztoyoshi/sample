#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <string>

#pragma comment( lib, "WS2_32.LIB" )

using namespace std;

static const int	UDP_TIMEWAIT	= 1;										//!< 1ms待ち
static const int	UDP_BUFFMAX		= 64+(256*5);								//!< UDP送受信サイズ(1344B)
static const int	UDP_BUFFARY		= 64;										//!< UDP配列数
int main( int argc, char **argv )
{
#if 1
	argv[ 1 ] = "192.168.1.2";			//!< MyIPADDR
	argv[ 2 ] = "44800";				//!< MyPORT
	argv[ 3 ] = "227.192.1.2";			//!< GroupIP
#endif

	// 初期設定
	string SMyIPAddr	= argv[ 1 ];
	string SMyPort		= argv[ 2 ];
	string SGroupIP		= argv[ 3 ];

	WSADATA wsaData;
	int nRes = -1;
	// winsock2の初期化
	nRes = ::WSAStartup( MAKEWORD( 2, 0 ), &wsaData );

	SOCKET			wsaSock;
	SOCKADDR_IN		sa_in;
	SOCKADDR_IN		sa_out;

	// UDP/IPソケットの作成
	wsaSock = ::WSASocket( AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0 );
	if( wsaSock == INVALID_SOCKET )
	{
		::printf( "WSASocket : %d\n", ::WSAGetLastError() );
		goto ERROR_SERVER;
	}

	DWORD ipaddr = ::inet_addr( SMyIPAddr.c_str() );
	if( ::setsockopt( wsaSock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ipaddr, sizeof( ipaddr ) ) != 0 )
	{
		::printf( "setsockopt IP_MULTICAST_IF : %d\n", ::WSAGetLastError() );
		goto ERROR_SERVER;
	}

	// アドレス（自分のローカルＩＰ）
	sa_in.sin_family			= AF_INET;
	sa_in.sin_port				= ::htons( ::atoi( SMyPort.c_str() ) );
	sa_in.sin_addr.S_un.S_addr	= ::inet_addr( SMyIPAddr.c_str() );

	// ソケットにローカルのＩＰとﾎﾟｰﾄを関連付け
	if( ::bind( wsaSock, reinterpret_cast<SOCKADDR*>( &sa_in ), sizeof( SOCKADDR ) ) != 0 )
	{
		::printf( "bind : %d\n", ::WSAGetLastError() );
		goto ERROR_SERVER;
	}

	const int ttl = 128;
	if( ::setsockopt( wsaSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof( ttl ) ) != 0 )
	{
		::printf( "setsockopt IP_MULTICAST_TTL : %d\n", ::WSAGetLastError() );
		goto ERROR_SERVER;
	}

	// アドレス（グループＩＰ）
	sa_out.sin_family			= AF_INET;
	sa_out.sin_port				= ::htons( ::atoi( SMyPort.c_str() ) );
	sa_out.sin_addr.S_un.S_addr	= ::inet_addr( SGroupIP.c_str() );

	// 
	char cBuf[ UDP_BUFFMAX ] = { '\0' };
	WSABUF wsaBuf;
	DWORD dwBytes = 0, dwFlags = 0;

	wsaBuf.buf	= cBuf;
	wsaBuf.len	= UDP_BUFFMAX;

	// 送受信イベント
	WSAOVERLAPPED wsaOver;
	WSANETWORKEVENTS wsaEvents;

	wsaOver.hEvent = ::WSACreateEvent();
	if( wsaOver.hEvent == NULL )
		goto ERROR_SERVER;

	if( ::WSAEventSelect( wsaSock, wsaOver.hEvent, FD_READ | FD_WRITE | FD_CLOSE ) != 0 )
		goto ERROR_SERVER;

	int nPolling = 0;
	while( 1 )
	{
		dwBytes = 0;
		nRes = ::WSAWaitForMultipleEvents( 1, &wsaOver.hEvent, FALSE, UDP_TIMEWAIT, FALSE );
		if( nRes == WSA_WAIT_FAILED )
			::shutdown( wsaSock, SD_SEND );

		::WSAEnumNetworkEvents( wsaSock, wsaOver.hEvent, &wsaEvents );

		// データの受信
		if( wsaEvents.lNetworkEvents & FD_READ )
		{
			::memset( wsaBuf.buf, 0, sizeof( wsaBuf.buf ) );

			nRes = ::WSARecvFrom( wsaSock, &wsaBuf, 1, &dwBytes, &dwFlags, NULL, NULL, NULL, NULL );
			if( nRes != SOCKET_ERROR )
			{
				// データの送信
				nRes = ::WSASendTo( wsaSock, &wsaBuf, 1, &dwBytes, dwFlags, reinterpret_cast<SOCKADDR*>( &sa_out ), sizeof( SOCKADDR ), NULL, NULL );
			}
		}

		nPolling++;
		if( nPolling % 10 == 0 )
			::printf_s( "." );
		::Sleep( 1 );
		::WSAResetEvent( wsaOver.hEvent );
	}
	::shutdown( wsaSock, SD_BOTH );
	::WSACloseEvent( wsaOver.hEvent );

ERROR_SERVER:
	::closesocket( wsaSock );

	::WSACleanup();
	return 0;
}
