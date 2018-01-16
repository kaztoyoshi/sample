#include "WLWnd.h"
namespace	WLWnd
{
namespace
{
	static const char*		WLWND_NAME		= "WINDOW_NAME";
	static const char*		WLWND_PROP		= "WINDOW_PROP";
	static const char*		ERROR_MUTEX		= "すでに起動しています。";
	static const int		nDefaultSizeX	= 640;
	static const int		nDefaultSizeY	= 480;
}

CWLWnd::CWLWnd( void )
	: m_hMutex( NULL )
	, m_hWnd( NULL )
	, m_hOwnWnd( NULL )
	, m_hInst( NULL )
	, m_hCur( NULL )
	, m_hIcon( NULL )
	, m_nBackground( 0xFFFFFF )
	, m_bEndFlag( FALSE )
	, m_nWndSizeX( nDefaultSizeX )
	, m_nWndSizeY( nDefaultSizeY )
	, m_nWndPosX( 0 )
	, m_nWndPosY( 0 )
	, m_dwWndStyle( WS_OVERLAPPED )
{
	strcpy_s( m_szWndName, WLWND_NAME );
	strcpy_s( m_szWndTitle, "" );
	strcpy_s( m_szWndMenu, "" );
	strcpy_s( m_szWndCursor, "" );
	strcpy_s( m_szWndIcon, "" );
}

CWLWnd::~CWLWnd( void )
{
	if( m_hWnd != NULL ) ::RemoveProp( m_hWnd, WLWND_PROP );
	if( m_hIcon != NULL ){ ::DestroyIcon( m_hIcon ); m_hIcon = NULL; }
	if( m_hCur != NULL ){ ::DestroyCursor( m_hCur ); m_hCur = NULL; }
	if( m_hOwnWnd != NULL ){ ::DestroyWindow( m_hOwnWnd ); m_hOwnWnd = NULL; }
	if( m_hWnd != NULL )
	{
		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
		::UnregisterClass( m_szWndName, NULL );
	}
	if( m_hMutex != NULL ){ ::CloseHandle( m_hMutex ); m_hMutex = NULL; }
}

BOOL CWLWnd::InitWindow( const HINSTANCE hInstance, const int nState, const bool bExFlag )
{
	m_hInst = hInstance;
	if( m_szWndIcon[0] != '\0' )
		m_hIcon = ::LoadIcon( m_hInst, m_szWndIcon );
	else
		m_hIcon = NULL;

	m_hMutex = ::CreateMutex( NULL, FALSE, m_szWndName );
	if( ::WaitForSingleObject( m_hMutex, 0 ) != WAIT_OBJECT_0 )
	{
		::CloseHandle( m_hMutex );
		return EXIT_FAILURE;
	}

	WNDCLASS			wc;														// ウインドウクラス
	wc.style				= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc			= static_cast<WNDPROC>( WndProc );					// メッセージを受け取るウインドウ関数
	wc.cbClsExtra			= 0;
	wc.cbWndExtra			= 0;
	wc.hInstance			= m_hInst;
	wc.hIcon				= m_hIcon;											// アイコン登録
	wc.hCursor				= m_hCur;											// カーソル登録
	wc.hbrBackground		= static_cast<HBRUSH>( ::CreateSolidBrush( m_nBackground ) );
	wc.lpszMenuName			= m_szWndMenu;										// メニューの登録
	wc.lpszClassName		= m_szWndName;
	if( !::RegisterClass( &wc ) ) return EXIT_FAILURE;

	m_hWnd = ::CreateWindow( m_szWndName,										// クラス名
							 m_szWndTitle,										// タイトル名
							 m_dwWndStyle,										// スタイル
							 m_nWndPosX,										// Ｘ座標
							 m_nWndPosY,										// Ｙ座標
							 m_nWndSizeX,										// 横幅
							 m_nWndSizeY,										// 高さ
							 m_hOwnWnd,											// 親ウインドウのハンドル
							 NULL,												// メニューハンドル
							 m_hInst,											// インスタンスハンドル
							 NULL );											// lParam
	RenewalWindow();

	if( m_szWndCursor[0] != '\0' )
		m_hCur = ::SetCursor( ::LoadCursor( m_hInst, m_szWndCursor ) );
	else
		m_hCur = ::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );

	if( !m_hWnd ) return EXIT_FAILURE;
	::ShowWindow( m_hWnd, nState );
	::UpdateWindow( m_hWnd );
	::SetProp( m_hWnd, WLWND_PROP, static_cast<HANDLE>( this ) );

	return EXIT_SUCCESS;
}

void CWLWnd::SetWindowName( const LPSTR lpName )
{
	strcpy_s( m_szWndName, lpName );
}

void CWLWnd::SetWindowTitle( const LPSTR lpTitle )
{
	strcpy_s( m_szWndTitle, lpTitle );
}

void CWLWnd::SetWindowStyle( const DWORD dwStyle )
{
	m_dwWndStyle = dwStyle;
}

void CWLWnd::SetWindowXYWH( const int nX, const int nY, const int nW, const int nH )
{
	m_nWndPosX	= nX;
	m_nWndPosY	= nY;
	m_nWndSizeX	= nW;
	m_nWndSizeY	= nH;
}

void CWLWnd::SetBackground( const int nR, const int nG, const int nB )
{
	m_nBackground = RGB( nR, nG, nB );
}

void CWLWnd::SetShowWindow( const int nState )
{
	::ShowWindow( m_hWnd, nState );
}

void CWLWnd::SetEndFlag( const BOOL bFlag )
{
	m_bEndFlag = bFlag;
}

const HWND CWLWnd::GetWindowHandle( void ) const
{
	return m_hWnd;
}

const BOOL CWLWnd::GetEndFlag( void ) const
{
	return m_bEndFlag;
}

void CWLWnd::RenewalWindow( void )
{
	RECT	rc[ 2 ];
	::GetWindowRect( m_hWnd, &rc[ 0 ] );
	::GetClientRect( m_hWnd, &rc[ 1 ] );

	::MoveWindow( m_hWnd,
				  rc[ 0 ].left,
				  rc[ 0 ].top,
				  m_nWndSizeX + ( rc[ 0 ].right - rc[ 0 ].left ) - ( rc[ 1 ].right - rc[ 1 ].left ),
				  m_nWndSizeY + ( rc[ 0 ].bottom - rc[ 0 ].top ) - ( rc[ 1 ].bottom - rc[ 1 ].top ),
				  TRUE );
}

void CWLWnd::RenewalWindow( const int nX, const int nY, const int nW, const int nH, const BOOL bFlag )
{
	m_nWndPosX = nX;
	m_nWndPosY = nY;
	m_nWndSizeX = nW;
	m_nWndSizeY = nH;

	::MoveWindow( m_hWnd, m_nWndPosX, m_nWndPosY, m_nWndSizeX, m_nWndSizeY, bFlag );
}

LRESULT CALLBACK CWLWnd::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CWLWnd *pTargetWnd = static_cast<CWLWnd*>( ::GetProp( hWnd, WLWND_PROP ) );
	if( pTargetWnd == NULL )
		return ::DefWindowProc( hWnd, uMsg, wParam, lParam );

	return pTargetWnd->WindowProc( hWnd, uMsg, wParam, lParam );
}

}