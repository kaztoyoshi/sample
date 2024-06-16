#include "NCRoom.h"

namespace	NetConfer
{

LRESULT CNCRoom::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_TIMER: Exec(); break;
	case WM_CLOSE:
	case WM_DESTROY:
		{
			m_CCon.SetEndFlag( TRUE );
			SetEndFlag( TRUE );
			::PostQuitMessage( 0 );													// WM_QUIT‚ð‘—‚é
		}
		break;
	default:
		return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	// ‚±‚Ì–½—ß‚ª‚È‚­‚È‚é‚ÆˆÚ“®EŠgk‚ª‚Å‚«‚È‚­‚È‚é
	return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	return 0L;
}

void CNCRoom::Exec( void )
{
	if( m_bCamera )
	{
		if( m_CCap.Play() )
		{
			if( !m_SJpg.bFlag )
			{
				m_SJpg.nTotal = m_CCap.Encode( m_SJpg.pData );
				m_SJpg.nStart = m_SJpg.nBlock = 0;
				m_SJpg.bFlag = TRUE;
			}
		}
	}
	if( m_CWavIn.CheckBuf() )
	{
		if( ( !m_SSnd.bFlag ) && ( m_CWavIn.CheckFlag() ) )
		{
			m_SSnd.nTotal = m_CWavIn.Encode( (char*)m_SSnd.pData );
			m_SSnd.nStart = m_SSnd.nBlock = 0;
			m_SSnd.bFlag = TRUE;
		}
	}

	Recv();
	Send();

	m_pGdi->ClearGDI();
	m_pGdi->ClearGDI( 0xFF, 0xFF, 0xFF );

	for( int i = 0; i < USER_MAX; i++ )
		m_pUser[ i ]->Draw();

	DrawLine();
	m_pGdi->DrawGDI();

	::Sleep( 1 );
}

void CNCRoom::Recv( void )
{
	BYTE byData[ UDP_BUFFMAX ];
	if( !m_CCon.UDPRead( byData ) ) return;

	int i;
	int nNumber = 0;
	char cNick[ NAME_MAX ] = { '\0' };
	::strncpy_s( cNick, NAME_MAX, (char *)( &byData[ 0 ] ), 32 );
	for( i = 0; i < USER_MAX; i++ )
	{
		if( m_pUser[ i ]->Check( cNick ) )
		{
			nNumber = i;
			break;
		}
	}
	if( i == USER_MAX ) return;

	int nTotal = ( byData[ 0x21 ] << 24 ) | ( byData[ 0x22 ] << 16 ) | ( byData[ 0x23 ] <<  8 ) | ( byData[ 0x24 ] <<  0 );
	int nStart = ( byData[ 0x25 ] << 24 ) | ( byData[ 0x26 ] << 16 ) | ( byData[ 0x27 ] <<  8 ) | ( byData[ 0x28 ] <<  0 );
	int nBlock = ( byData[ 0x29 ] << 24 ) | ( byData[ 0x2A ] << 16 ) | ( byData[ 0x2B ] <<  8 ) | ( byData[ 0x2C ] <<  0 );

	if( byData[ 0x20 ] == 1 )
		m_pUser[ nNumber ]->Audio( nTotal, nStart, nBlock, byData );
	else
	if( byData[ 0x20 ] == 2 )
		m_pUser[ nNumber ]->Video( nTotal, nStart, nBlock, byData );
}

void CNCRoom::Send( void )
{
	BYTE byData[ UDP_BUFFMAX ];

	for( int i = 0; i < 32; i++ )
	{
		byData[ i ] = m_szNickName[ i ];
		if( m_szNickName[ i ] == '\0' ) break;
	}
	byData[ 0x1F ] = '\0';

	if( m_SSnd.bFlag )
	{
		byData[ 0x20 ] = 1;
		byData[ 0x21 ] = (BYTE)( m_SSnd.nTotal >> 24 );
		byData[ 0x22 ] = (BYTE)( m_SSnd.nTotal >> 16 );
		byData[ 0x23 ] = (BYTE)( m_SSnd.nTotal >>  8 );
		byData[ 0x24 ] = (BYTE)( m_SSnd.nTotal >>  0 );

		for( int i = 0; i < 8; i++ )
		{
			if( m_SSnd.nTotal - m_SSnd.nStart > BLOCK_MAX )
				m_SSnd.nBlock = BLOCK_MAX;
			else
				m_SSnd.nBlock = m_SSnd.nTotal - m_SSnd.nStart;

			byData[ 0x25 ] = (BYTE)( m_SSnd.nStart >> 24 );
			byData[ 0x26 ] = (BYTE)( m_SSnd.nStart >> 16 );
			byData[ 0x27 ] = (BYTE)( m_SSnd.nStart >>  8 );
			byData[ 0x28 ] = (BYTE)( m_SSnd.nStart >>  0 );
			byData[ 0x29 ] = (BYTE)( m_SSnd.nBlock >> 24 );
			byData[ 0x2A ] = (BYTE)( m_SSnd.nBlock >> 16 );
			byData[ 0x2B ] = (BYTE)( m_SSnd.nBlock >>  8 );
			byData[ 0x2C ] = (BYTE)( m_SSnd.nBlock >>  0 );

			::memcpy_s( &byData[ 0x40 ], m_SSnd.nBlock, &m_SSnd.pData[ m_SSnd.nStart ], m_SSnd.nBlock );

			m_CCon.UDPWrite( byData );

			m_SSnd.nStart += m_SSnd.nBlock;
			if( m_SSnd.nTotal <= m_SSnd.nStart )
			{
				m_SSnd.bFlag = FALSE;
				break;
			}
		}
	}
	if( m_SJpg.bFlag )
	{
		byData[ 0x20 ] = 2;
		byData[ 0x21 ] = (BYTE)( m_SJpg.nTotal >> 24 );
		byData[ 0x22 ] = (BYTE)( m_SJpg.nTotal >> 16 );
		byData[ 0x23 ] = (BYTE)( m_SJpg.nTotal >>  8 );
		byData[ 0x24 ] = (BYTE)( m_SJpg.nTotal >>  0 );

		for( int i = 0; i < 8; i++ )
		{
			if( m_SJpg.nTotal - m_SJpg.nStart > BLOCK_MAX )
				m_SJpg.nBlock = BLOCK_MAX;
			else
				m_SJpg.nBlock = m_SJpg.nTotal - m_SJpg.nStart;

			byData[ 0x25 ] = (BYTE)( m_SJpg.nStart >> 24 );
			byData[ 0x26 ] = (BYTE)( m_SJpg.nStart >> 16 );
			byData[ 0x27 ] = (BYTE)( m_SJpg.nStart >>  8 );
			byData[ 0x28 ] = (BYTE)( m_SJpg.nStart >>  0 );
			byData[ 0x29 ] = (BYTE)( m_SJpg.nBlock >> 24 );
			byData[ 0x2A ] = (BYTE)( m_SJpg.nBlock >> 16 );
			byData[ 0x2B ] = (BYTE)( m_SJpg.nBlock >>  8 );
			byData[ 0x2C ] = (BYTE)( m_SJpg.nBlock >>  0 );

			::memcpy_s( &byData[ 0x40 ], m_SJpg.nBlock, &m_SJpg.pData[ m_SJpg.nStart ], m_SJpg.nBlock );

			m_CCon.UDPWrite( byData );

			m_SJpg.nStart += m_SJpg.nBlock;
			if( m_SJpg.nTotal <= m_SJpg.nStart )
			{
				m_SJpg.bFlag = FALSE;
				break;
			}
		}
	}
}

void CNCRoom::DrawLine( void )
{
	HPEN hPen = static_cast<HPEN>( ::SelectObject( m_hdcWork, m_hPen ) );
	::MoveToEx( m_hdcWork, 0, 120, NULL );
	::LineTo( m_hdcWork, 480, 120 );
	::MoveToEx( m_hdcWork, 0, 240, NULL );
	::LineTo( m_hdcWork, 480, 240 );
	::MoveToEx( m_hdcWork, 160, 0, NULL );
	::LineTo( m_hdcWork, 160, 360 );
	::MoveToEx( m_hdcWork, 320, 0, NULL );
	::LineTo( m_hdcWork, 320, 360 );
	::SelectObject( m_hdcWork, hPen );
}

CNCRoom::CNCRoom( void )
	: m_pGdi( NULL )
	, m_hWnd( NULL )
	, m_hdcWork()
	, m_hPen( NULL )
	, m_CCon()
	, m_CCap()
	, m_bCamera( FALSE )
	, m_CWavIn()
	, m_SJpg()
	, m_SSnd()
	, m_CUser0()
	, m_CUser1()
	, m_CUser2()
	, m_CUser3()
	, m_CUser4()
	, m_CUser5()
	, m_CUser6()
	, m_CUser7()
	, m_CUser8()
{
	m_pUser[ 0 ] = &m_CUser0;
	m_pUser[ 1 ] = &m_CUser1;
	m_pUser[ 2 ] = &m_CUser2;
	m_pUser[ 3 ] = &m_CUser3;
	m_pUser[ 4 ] = &m_CUser4;
	m_pUser[ 5 ] = &m_CUser5;
	m_pUser[ 6 ] = &m_CUser6;
	m_pUser[ 7 ] = &m_CUser7;
	m_pUser[ 8 ] = &m_CUser8;
}

CNCRoom::~CNCRoom( void )
{
	m_CCon.End();
	::KillTimer( m_hWnd, ID_TIMER );

	for( int i = 0; i < USER_MAX; i++ )
		m_pUser[ i ]->End();
	if( m_bCamera )
		m_CCap.Release();
	m_CWavIn.End();
	::Sleep( 100 );
	if( m_SSnd.pData != NULL )	::free( m_SSnd.pData );
	if( m_SJpg.pData != NULL )	::free( m_SJpg.pData );
	if( m_hPen != NULL )		::DeleteObject( m_hPen );
	if( m_pGdi != NULL )		delete m_pGdi;
}

BOOL CNCRoom::Init( const HINSTANCE& hInstance, const char* cAddr, const int& nPort, const char* cNickName )
{
	int nSizeW = WINDOW_WIDTH + ::GetSystemMetrics( SM_CXFIXEDFRAME );
	int nSizeH = WINDOW_HEIGHT + ( ::GetSystemMetrics( SM_CYCAPTION ) + ::GetSystemMetrics( SM_CYFIXEDFRAME ) );
	int nPosX = ( ::GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH )  / 2;
	int nPosY = ( ::GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2;

	SetWindowXYWH( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
	RenewalWindow( nPosX, nPosY, WINDOW_WIDTH, WINDOW_HEIGHT );
	SetWindowStyle( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU );
	SetBackground( 0xFF, 0xFF, 0xFF );
	SetWindowName( PROCNAME );
	SetWindowTitle( PROCNAME );

	if( InitWindow( hInstance ) == EXIT_FAILURE )
		return EXIT_FAILURE;

	SetShowWindow( SW_SHOW );

	m_hWnd = GetWindowHandle();

	m_pGdi = new WLGdi::CWLGdi( m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT );
	m_pGdi->InitGDI();

	m_hdcWork = m_pGdi->GetWorkGDI();

	::strcpy_s( m_cAddr, TEXT_MAX, cAddr );
	m_nPort = nPort;
	::strcpy_s( m_szNickName, NAME_MAX, cNickName );

	m_bCamera = m_CCap.Init( m_szNickName, nPosX - ROOM_WIDTH, nPosY, m_hWnd );
	m_CWavIn.Init();
	if( m_CCon.Start( m_cAddr, nPort ) == FALSE )
		return EXIT_FAILURE;
	m_hPen = ::CreatePen( PS_SOLID, 1, RGB( 0x80, 0x80, 0x80 ) );
	m_SJpg.pData = (BYTE*)::malloc( ROOM_WIDTH * ROOM_HEIGHT * 3 );
	m_SSnd.pData = (BYTE*)::malloc( 44100 * 2 * 2 );

	m_pHost = m_pUser[ 0 ];
	for( int i = 0; i < USER_MAX; i++ )
	{
		int nX = i % 3;
		int nY = i / 3;
		m_pUser[ i ]->Init( m_hdcWork, ROOM_WIDTH * nX, ROOM_HEIGHT * nY );
	}

	::SetTimer( m_hWnd, ID_TIMER, TIMEOUT_VALUE, NULL );

	return EXIT_SUCCESS;
}

void CNCRoom::End( void )
{
}

}