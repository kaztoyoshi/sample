#include "NCCamera.h"

namespace	NetConfer
{
unsigned __stdcall CNCCamera::InitThread( void *pVoid )
{
	CNCCamera *pCam = reinterpret_cast<CNCCamera*>( pVoid );

	pCam->cvInit( pCam->m_rcSize, pCam->m_cNick, pCam->m_hOwn );
	pCam->m_bExec = TRUE;

	::_endthreadex( 0 );
	return TRUE;
}

CNCCamera::CNCCamera( void )
	: m_hModule( NULL )
	, m_bExec( FALSE )
	, m_nTimer( 0 )
	, m_hOwn( NULL )
	, m_rcSize()
{
}

CNCCamera::~CNCCamera( void )
{
}

BOOL CNCCamera::Init( const char* cNick, const int& nX, const int& nY, const HWND& hOwn )
{
	BOOL bFlag = FALSE;
	::strcpy_s( m_cNick, NAME_MAX, cNick );
	m_hOwn = hOwn;
	m_rcSize.left	= nX;
	m_rcSize.top	= nY;
	m_rcSize.right	= CAMERA_WIDTH;
	m_rcSize.bottom	= CAMERA_HEIGHT;

	m_hModule = ::LoadLibrary( "OpenCV.dll" );
	if( m_hModule != NULL )
	{
		cvInit = (CV_INIT)::GetProcAddress( m_hModule, "CV_INIT" );
		cvPlay = (CV_PLAY)::GetProcAddress( m_hModule, "CV_PLAY" );
		cvEncode = (CV_ENCODE)::GetProcAddress( m_hModule, "CV_ENCODE" );
		cvDecode = (CV_DECODE)::GetProcAddress( m_hModule, "CV_DECODE" );
		cvRelease = (CV_RELEASE)::GetProcAddress( m_hModule, "CV_RELEASE" );

		unsigned int uiID;
		HANDLE hID = reinterpret_cast<HANDLE>( ::_beginthreadex( NULL, 0, &CNCCamera::InitThread, this, 0, &uiID ) );
		::CloseHandle( hID );
	}

	return TRUE;
}

BOOL CNCCamera::Play( void )
{
	if( !m_bExec ) return FALSE;
	BOOL bFlag = FALSE;
	if( m_nTimer++ % TIMER_COUNT == 0 )
		bFlag = cvPlay();
	return bFlag;
}

int CNCCamera::Encode( unsigned char* p )
{
	if( !m_bExec ) return FALSE;
	return cvEncode( p );
}

int CNCCamera::Decode( unsigned char* src, unsigned char* dst )
{
	return cvDecode( src, dst );
}

BOOL CNCCamera::Release( void )
{
	BOOL bFlag = FALSE;
	if( m_hModule != NULL )
		bFlag = cvRelease();
	::FreeLibrary( m_hModule );
	return bFlag;
}

}