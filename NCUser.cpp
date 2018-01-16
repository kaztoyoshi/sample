#include "NCUser.h"
#include "NCCamera.h"

namespace	NetConfer
{
CNCUser::CNCUser( void )
	: m_bUsed( FALSE )
	, m_hDC()
	, m_nPosX( 0 )
	, m_nPosY( 0 )
	, m_Bmi()
	, m_nDispRead( 0 )
	, m_nDispWrite( 0 )
	, m_pCamera( NULL )
	, m_hModule( NULL )
	, m_CWavOut()
	, m_pAudio( NULL )
{
	m_hBmp[ 0 ] = NULL;
	m_hBmp[ 1 ] = NULL;
	m_lpBuf[ 0 ] = NULL;
	m_lpBuf[ 1 ] = NULL;
}

CNCUser::~CNCUser( void )
{
	::FreeLibrary( m_hModule );
	if( m_pAudio != NULL )			::free( m_pAudio );
	if( m_pCamera != NULL )			::free( m_pCamera );

	if( m_hBmp[ 1 ] != NULL )		::DeleteObject( m_hBmp[ 1 ] );
	if( m_hBmp[ 0 ] != NULL )		::DeleteObject( m_hBmp[ 0 ] );
}

void CNCUser::Init( const HDC& hdcWork, const int& nPosX, const int& nPosY )
{
	m_bUsed = FALSE;
	m_hDC = hdcWork;
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	// 
	m_Bmi.bmiHeader.biSize			= sizeof( m_Bmi.bmiHeader );
	m_Bmi.bmiHeader.biWidth			= CNCCamera::CAMERA_WIDTH;
	m_Bmi.bmiHeader.biHeight		= -CNCCamera::CAMERA_HEIGHT;
	m_Bmi.bmiHeader.biPlanes		= 1;
	m_Bmi.bmiHeader.biBitCount		= 24;
	m_Bmi.bmiHeader.biCompression	= BI_RGB;
	m_Bmi.bmiHeader.biSizeImage		= ( CNCCamera::CAMERA_WIDTH * CNCCamera::CAMERA_HEIGHT * ( m_Bmi.bmiHeader.biBitCount / 8 ) );
	m_Bmi.bmiHeader.biXPelsPerMeter	= 0;
	m_Bmi.bmiHeader.biYPelsPerMeter	= 0;
	m_Bmi.bmiHeader.biClrUsed		= 0;
	m_Bmi.bmiHeader.biClrImportant	= 0;

	m_hBmp[ 0 ] = ::CreateDIBSection( NULL, &m_Bmi, DIB_RGB_COLORS, reinterpret_cast<void**>( &m_lpBuf[ 0 ] ), NULL, 0 );
	m_hBmp[ 1 ] = ::CreateDIBSection( NULL, &m_Bmi, DIB_RGB_COLORS, reinterpret_cast<void**>( &m_lpBuf[ 1 ] ), NULL, 0 );

	::memset( m_lpBuf[ 0 ], 0xFF, m_Bmi.bmiHeader.biSizeImage );
	::memset( m_lpBuf[ 1 ], 0xFF, m_Bmi.bmiHeader.biSizeImage );

	m_pCamera = (BYTE*)::malloc( CNCCamera::CAMERA_WIDTH * CNCCamera::CAMERA_HEIGHT * 3 );

	m_CWavOut.Init();
	m_pAudio = (BYTE*)::malloc( m_CWavOut.GetSize() * sizeof( float ) );

	m_hModule = ::LoadLibrary( "OpenCV.dll" );
	if( m_hModule != NULL )
		cvDDecode = (CV_DDECODE)::GetProcAddress( m_hModule, "CV_DDECODE" );
}

void CNCUser::Draw( void )
{
	if( !m_bUsed ) return;

	if( m_nDispRead < m_nDispWrite )
		m_nDispRead++;
	int nRead = m_nDispRead % 2;

	::StretchDIBits( m_hDC, m_nPosX, m_nPosY, CNCCamera::CAMERA_WIDTH, CNCCamera::CAMERA_HEIGHT,
					0, 0, CNCCamera::CAMERA_WIDTH, CNCCamera::CAMERA_HEIGHT,
					m_lpBuf[ nRead ], &m_Bmi, DIB_RGB_COLORS, SRCCOPY );
}

BOOL CNCUser::Check( const char* cNick )
{
	BOOL bFlag = FALSE;
	if( m_bUsed )
	{
		if( ::strcmp( m_szNickName, cNick ) == 0 )
			bFlag = TRUE;
	}else
	{
		::strcpy_s( m_szNickName, NAME_MAX, cNick );
		m_bUsed = TRUE;
		bFlag = TRUE;
	}
	return bFlag;
}

void CNCUser::Video( const int& nTotal, const int& nStart,const int& nBlock, const BYTE byData[] )
{
	::memcpy_s( &m_pCamera[ nStart ], nBlock, &byData[ 0x40 ], nBlock );

	if( nStart + nBlock >= nTotal )
	{
		int nWrite = m_nDispWrite % 2;
		cvDDecode( CNCCamera::CAMERA_WIDTH, CNCCamera::CAMERA_HEIGHT, m_pCamera, m_lpBuf[ nWrite ] );
		m_nDispWrite++;
	}
}

void CNCUser::Audio( const int& nTotal, const int& nStart, const int& nBlock, const BYTE byData[] )
{
	::memcpy_s( &m_pAudio[ nStart ], nBlock, &byData[ 0x40 ], nBlock );

	if( nStart + nBlock >= nTotal )
		m_CWavOut.Decode( (char*)m_pAudio, nTotal );
}

}