#include "NCConnect.h"

namespace	NetConfer
{
unsigned __stdcall CNCConnect::UDPConnect( void *pVoid )
{
	CNCConnect *p = reinterpret_cast<CNCConnect*>( pVoid );

	SOCKET wsaSock;

	wsaSock = ::WSASocket( AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0 );
	if( wsaSock == INVALID_SOCKET )
	{
		ASSERT( 0 && "UDPServer : WSASocket" );
		goto ERROR_CONNECT;
	}

	char hostname[ MAXGETHOSTSTRUCT + 1 ] = {'\0'};
	::gethostname( hostname, sizeof( hostname ) );

	LPHOSTENT host;
	host = ::gethostbyname( hostname );

	char cAddr[ TEXT_MAX ];
	::sprintf_s( cAddr, TEXT_MAX, "%d.%d.%d.%d",
		static_cast<BYTE>( *( ( host->h_addr_list[ 0 ] ) + 0 ) ),
		static_cast<BYTE>( *( ( host->h_addr_list[ 0 ] ) + 1 ) ),
		static_cast<BYTE>( *( ( host->h_addr_list[ 0 ] ) + 2 ) ),
		static_cast<BYTE>( *( ( host->h_addr_list[ 0 ] ) + 3 ) ) );


	SOCKADDR_IN sa_in;
	sa_in.sin_family			= AF_INET;
	sa_in.sin_port				= ::htons( p->m_nPort );
	sa_in.sin_addr.S_un.S_addr	= ::inet_addr( cAddr );

	if( ::bind( wsaSock, reinterpret_cast<SOCKADDR*>( &sa_in ), sizeof( SOCKADDR ) ) != 0 )
	{
		::printf( "bind : %d\n", ::WSAGetLastError() );
		goto ERROR_CONNECT;
	}

	static ip_mreq mreq;
	::memset( &mreq, 0, sizeof( mreq ) );
	mreq.imr_interface.S_un.S_addr = sa_in.sin_addr.S_un.S_addr;
	mreq.imr_multiaddr.S_un.S_addr = ::inet_addr( p->m_cAddr );

	if( ::setsockopt( wsaSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof( mreq ) ) != 0 )
	{
		::printf( "setsockopt IP_ADD_MEMBERSHIP : %d\n", ::WSAGetLastError() );
		goto ERROR_CONNECT;
	}

	const int ttl = 128;
	if( ::setsockopt( wsaSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof( ttl ) ) != 0 )
	{
		::printf( "setsockopt IP_MULTICAST_TTL : %d\n", ::WSAGetLastError() );
		goto ERROR_CONNECT;
	}

	SOCKADDR_IN sa_out;
	sa_out.sin_family			= AF_INET;
	sa_out.sin_port				= ::htons( p->m_nPort );
	sa_out.sin_addr.S_un.S_addr	= ::inet_addr( p->m_cAddr );

	char cBuf[ UDP_BUFFMAX ] = { "\0" };
	WSABUF wsaBuf;
	DWORD dwBytes = 0, dwFlags = 0;

	wsaBuf.buf = cBuf;
	wsaBuf.len = UDP_BUFFMAX;

	WSAOVERLAPPED wsaOver;
	WSANETWORKEVENTS wsaEvents;

	wsaOver.hEvent = ::WSACreateEvent();
	if( wsaOver.hEvent == NULL )
		goto ERROR_CONNECT;

	if( ::WSAEventSelect( wsaSock, wsaOver.hEvent, FD_READ ) != 0 )
		goto ERROR_CONNECT;

	p->m_bConn = TRUE;
	int nRes = -1;
	while( 1 )
	{
		dwBytes = 0;
		nRes = ::WSAWaitForMultipleEvents( 1, &wsaOver.hEvent, FALSE, UDP_TIMEWAIT, FALSE );
		if( nRes == WSA_WAIT_FAILED )
			::shutdown( wsaSock, SD_SEND );

		::WSAEnumNetworkEvents( wsaSock, wsaOver.hEvent, &wsaEvents );

		if( p->m_nUSRead < p->m_nUSWrite )
		{
			int nRead = p->m_nUSRead % UDP_BUFFARY;
			int nWrite = p->m_nUSWrite % UDP_BUFFARY;

			wsaBuf.buf = reinterpret_cast<CHAR*>( p->m_byUDPSend[ nRead ] );
			wsaBuf.len = UDP_BUFFMAX;

			nRes = ::WSASendTo( wsaSock, &wsaBuf, 1, &dwBytes, dwFlags, reinterpret_cast<SOCKADDR*>( &sa_out ), sizeof( SOCKADDR ), NULL, NULL );
			if( nRes != SOCKET_ERROR )
				p->m_nUSRead++;
		}

		if( wsaEvents.lNetworkEvents & FD_READ )
		{
			int nRead = p->m_nURRead % UDP_BUFFARY;
			int nWrite = p->m_nURWrite % UDP_BUFFARY;

			wsaBuf.buf = reinterpret_cast<CHAR*>( &p->m_byUDPRecv[ nWrite ] );
			wsaBuf.len = UDP_BUFFMAX;
			::memset( wsaBuf.buf, 0, sizeof( wsaBuf.buf ) );

			SOCKADDR_IN		sa_in;
			int AddrSize = sizeof(SOCKADDR);
			nRes = ::WSARecvFrom( wsaSock, &wsaBuf, 1, &dwBytes, &dwFlags, reinterpret_cast<SOCKADDR*>( &sa_in ), &AddrSize, NULL, NULL );
			if( nRes != SOCKET_ERROR )
				p->m_nURWrite++;
		}

		if( p->m_bEndFlag )
			break;

		::Sleep( 1 );
		::WSAResetEvent( wsaOver.hEvent );
	}
	p->m_bConn = FALSE;
	::shutdown( wsaSock, SD_BOTH );
	::WSACloseEvent( wsaOver.hEvent );

	if( ::setsockopt( wsaSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof( mreq ) ) != 0 )
	{
		::printf( "setsockopt IP_DROP_MEMBERSHIP : %d\n", ::WSAGetLastError() );
		goto ERROR_CONNECT;
	}

ERROR_CONNECT:
	::closesocket( wsaSock );

	::_endthreadex( 0 );
	return TRUE;
}

CNCConnect::CNCConnect( void )
	: m_wsaData()
	, m_nRes( -1 )
	, m_bEndFlag( FALSE )
	, m_bConn( FALSE )
	, m_nPort( 0 )
	, m_nURRead( 0 )
	, m_nURWrite( 0 )
	, m_nUSRead( 0 )
	, m_nUSWrite( 0 )
{
	m_nRes = ::WSAStartup( MAKEWORD( 2, 0 ), &m_wsaData );
}

CNCConnect::~CNCConnect( void )
{
	::WSACleanup();
}

BOOL CNCConnect::Start( const char* cAddr, const int& nPort )
{
	::strcpy_s( m_cAddr, TEXT_MAX, cAddr );
	m_nPort = nPort;

	if( m_nRes == 0 )
	{
		unsigned int uiID;
		HANDLE hID = reinterpret_cast<HANDLE>( ::_beginthreadex( NULL, 0, &CNCConnect::UDPConnect, this, 0, &uiID ) );
		::CloseHandle( hID );
		return TRUE;
	}
	return FALSE;
}

void CNCConnect::End( void )
{
	for( int i = 0; i < 100; i++ )
	{
		if( m_bEndFlag ) break;
	}
}

BOOL CNCConnect::UDPRead( BYTE byData[] )
{
	if( m_nURRead < m_nURWrite )
	{
		int nRead = m_nURRead % UDP_BUFFARY;
		::memcpy_s( byData, UDP_BUFFMAX, m_byUDPRecv[ nRead ], UDP_BUFFMAX );
		m_nURRead++;
	}
	return ( m_nURRead < m_nURWrite ) ? TRUE : FALSE;
}

void CNCConnect::UDPWrite( const BYTE byData[] )
{
	if( !m_bConn ) return;
	int nWrite = m_nUSWrite % UDP_BUFFARY;
	::memcpy_s( m_byUDPSend[ nWrite ], UDP_BUFFMAX, byData, UDP_BUFFMAX );
	m_nUSWrite++;
}

}