#include "NCAudio.h"
#include <math.h>

namespace	NetConfer
{

void CALLBACK CNCWavIn::waveInProc( HWAVEIN hwi, UINT uMsg, DWORD dwInst, DWORD dwParam1, DWORD dwParam2 )
{
	CNCWavIn *pIn = reinterpret_cast<CNCWavIn*>( dwInst );
	SWav *p = &pIn->m_SWav;
	if( !p->m_bFlag ) return;

	switch( uMsg )
	{
	case WIM_DATA:
		{
			LPWAVEHDR	lpwvhdr = reinterpret_cast<LPWAVEHDR>( dwParam1 );

			int nWrite = p->m_nWrite % BUFFER_MAX;
			::memcpy_s( p->m_pData[ nWrite ], lpwvhdr->dwBytesRecorded, lpwvhdr->lpData, lpwvhdr->dwBytesRecorded );
			p->m_nWrite++;

			::waveInUnprepareHeader( p->m_HWI, lpwvhdr, sizeof( WAVEHDR ) );
			::waveInPrepareHeader( p->m_HWI, lpwvhdr, sizeof( WAVEHDR ) );
			::waveInAddBuffer( p->m_HWI, lpwvhdr, sizeof( WAVEHDR ) );
		}
		break;
	}
}

unsigned __stdcall CNCWavIn::WaveIn( void *pVoid )
{
	CNCWavIn *pIn = reinterpret_cast<CNCWavIn*>( pVoid );
	SWav *p = &pIn->m_SWav;
	p->m_bState = TRUE;

	if( ::waveInOpen( &p->m_HWI, WAVE_MAPPER, &p->m_WFEX, reinterpret_cast<DWORD_PTR>( waveInProc ), reinterpret_cast<DWORD_PTR>( pIn ), CALLBACK_FUNCTION ) != MMSYSERR_NOERROR )
		goto WAV_ERROR;

	for( int i = 0; i < BUFFER_MAX; i++ )
	{
		p->m_WH[ i ].lpData			= reinterpret_cast<LPSTR>( ::HeapAlloc( ::GetProcessHeap(), 0, p->m_WFEX.nAvgBytesPerSec * p->m_dwSecond ) );
		p->m_WH[ i ].dwFlags		= 0;
		p->m_WH[ i ].dwBufferLength	= p->m_WFEX.nAvgBytesPerSec * p->m_dwSecond;

		::waveInPrepareHeader( p->m_HWI, &p->m_WH[ i ], sizeof( WAVEHDR ) );
		::waveInAddBuffer( p->m_HWI, &p->m_WH[ i ], sizeof( WAVEHDR ) );
	}

	::waveInStart( p->m_HWI );
	p->m_bFlag = TRUE;

	int nSndSize = p->m_WFEX.nAvgBytesPerSec * p->m_dwSecond;
	pIn->m_nBufSize = nSndSize / 4;

	for( int i = 0; i < BUFFER_MAX; i++ )
	{
		pIn->m_pBuf[ i ][ 0 ] = (float*)::malloc( pIn->m_nBufSize * sizeof( float ) );
		pIn->m_pBuf[ i ][ 1 ] = (float*)::malloc( pIn->m_nBufSize * sizeof( float ) );
	}

	while( 1 )
	{
		if( p->m_nRead < p->m_nWrite )
		{
			int nCnt = 0;
			int nWrite = pIn->m_nWrite % BUFFER_MAX;
			int nRead = p->m_nRead % BUFFER_MAX;
			p->m_nRead++;

			for( nCnt = 0; nCnt < pIn->m_nBufSize; nCnt++ )
			{
				pIn->m_pBuf[ nWrite ][ 0 ][ nCnt ]
						= ( ( ( p->m_pData[ nRead ][ nCnt * 4 + 1 ] << 8 )
						|     ( p->m_pData[ nRead ][ nCnt * 4 + 0 ] & 0x00FF ) )
						/ 32768.f );
				pIn->m_pBuf[ nWrite ][ 1 ][ nCnt ]
						= ( ( ( p->m_pData[ nRead ][ nCnt * 4 + 3 ] << 8 )
						|     ( p->m_pData[ nRead ][ nCnt * 4 + 2 ] & 0x00FF ) )
						/ 32768.f );
			}
			pIn->m_nWrite++;
		}
		::Sleep( 10 );
		if( !p->m_bState ) break;
	}

	p->m_bFlag = FALSE;
	::waveInStop( p->m_HWI );

	for( int i = 0; i < BUFFER_MAX; i++ )
		::waveInUnprepareHeader( p->m_HWI, &p->m_WH[ i ], sizeof( WAVEHDR ) );

	::waveInReset( p->m_HWI );
	::waveInClose( p->m_HWI );

	for( int i = 0; i < BUFFER_MAX; i++ )
	{
		::free( pIn->m_pBuf[ i ][ 0 ] );
		::free( pIn->m_pBuf[ i ][ 1 ] );
	}

	for( int i = 0; i < BUFFER_MAX; i++ )
		::HeapFree( ::GetProcessHeap(), 0, p->m_WH[ i ].lpData );

WAV_ERROR:
	p->m_bState = FALSE;
	::_endthreadex( 0 );
	return TRUE;
}

CNCWavIn::CNCWavIn( void )
	: m_SWav()
	, m_nBufSize( 0 )
	, m_nWrite( 0 )
	, m_nRead( 0 )
{
	for( int i = 0; i < BUFFER_MAX; i++ )
	{
		m_SWav.m_pData[ i ] = reinterpret_cast<char*>( ::malloc( m_SWav.m_WFEX.nAvgBytesPerSec * m_SWav.m_dwSecond ) );
		m_pBuf[ i ][ 0 ] = NULL;
		m_pBuf[ i ][ 1 ] = NULL;
	}
}

CNCWavIn::~CNCWavIn( void )
{
	for( int i = 0; i < BUFFER_MAX; i++ )
		if( m_SWav.m_pData[ i ] != NULL ) ::free( m_SWav.m_pData[ i ] );
}

void CNCWavIn::Init( void )
{
	unsigned int uiID;
	HANDLE hID;
	hID = reinterpret_cast<HANDLE>( ::_beginthreadex( NULL, 0, &WaveIn, this, 0, &uiID ) );
	::CloseHandle( hID );
}

int CNCWavIn::Encode( char *p )
{
	int nRead = m_nRead % BUFFER_MAX;
	m_nRead++;

	for( int nCnt = 0; nCnt < m_nBufSize; nCnt++ )
	{
		for( int i = 0; i < 2; i++ )
		{
			int nPos = nCnt * 2 * 4 + i * 4;
			char *c = (char*)&m_pBuf[ nRead ][ i ][ nCnt ];
			p[ nPos + 0 ] = c[ 0 ];
			p[ nPos + 1 ] = c[ 1 ];
			p[ nPos + 2 ] = c[ 2 ];
			p[ nPos + 3 ] = c[ 3 ];
		}
	}

	return ( m_nBufSize * 2 );
}

unsigned __stdcall CNCWavOut::WaveOut( void *pVoid )
{
	CNCWavOut *pOut = reinterpret_cast<CNCWavOut*>( pVoid );
	SWav *p = &pOut->m_SWav;
	p->m_bState = TRUE;

	if( ::waveOutOpen( &p->m_HWO, WAVE_MAPPER, &p->m_WFEX, NULL, NULL, 0 ) != MMSYSERR_NOERROR )
		goto OGG_ERROR;

	for( int i = 0; i < BUFFER_MAX; i++ )
	{
		p->m_WH[ i ].lpData			= reinterpret_cast<LPSTR>( ::HeapAlloc( ::GetProcessHeap(), 0, p->m_WFEX.nAvgBytesPerSec * p->m_dwSecond ) );
		p->m_WH[ i ].dwFlags		= 0;
		p->m_WH[ i ].dwBufferLength	= p->m_WFEX.nAvgBytesPerSec * p->m_dwSecond;

		::waveOutPrepareHeader( p->m_HWO, &p->m_WH[ i ], sizeof( WAVEHDR ) );
	}

	p->m_bFlag = TRUE;

	while( 1 )
	{
		if( p->m_nRead < p->m_nWrite )
		{
			int nRead = p->m_nRead % BUFFER_MAX;
			p->m_nRead++;

			::waveOutUnprepareHeader( p->m_HWO, &p->m_WH[ nRead ], sizeof( WAVEHDR ) );
			::waveOutPrepareHeader( p->m_HWO, &p->m_WH[ nRead ], sizeof( WAVEHDR ) );
			::waveOutWrite( p->m_HWO, &p->m_WH[ nRead ], sizeof( WAVEHDR ) );
		}
		Sleep( 10 );
		if( !p->m_bState ) break;
	}

	p->m_bFlag = FALSE;

	for( int i = 0; i < BUFFER_MAX; i++ )
		::waveOutUnprepareHeader( p->m_HWO, &p->m_WH[ i ], sizeof( WAVEHDR ) );

	::waveOutReset( p->m_HWO );
	::waveOutClose( p->m_HWO );

	for( int i = 0; i < BUFFER_MAX; i++ )
		::HeapFree( ::GetProcessHeap(), 0, p->m_WH[ i ].lpData );

OGG_ERROR:
	p->m_bState = FALSE;
	::_endthreadex( 0 );
	return TRUE;
}

float CNCWavOut::GetIEEE754( const unsigned char *buf )
{
	int sign = ( buf[ 3 ] & 0x80 ) ? -1 : 1;
	int exp = ( ( buf[ 3 ] & 0x7f ) << 1 | ( buf[ 2 ] >> 7 ) );
	int mantissa = ( ( buf[ 2 ] & 0x7f ) << 16 ) | ( buf[ 1 ] << 8 ) | buf[ 0 ];

	return ( sign * ::pow( 2.0f, exp - 127 ) * ( ( mantissa | 0x800000 ) / 8388608.f ) );
}

CNCWavOut::CNCWavOut( void )
	: m_SWav()
{
	for( int i = 0; i < BUFFER_MAX; i++ )
		m_SWav.m_pData[ i ] = reinterpret_cast<char*>( ::malloc( m_SWav.m_WFEX.nAvgBytesPerSec * m_SWav.m_dwSecond ) );
}

CNCWavOut::~CNCWavOut( void )
{
	for( int i = 0; i < BUFFER_MAX; i++ )
		if( m_SWav.m_pData[ i ] != NULL ) ::free( m_SWav.m_pData[ i ] );
}

void CNCWavOut::Init( void )
{
	unsigned int uiID;
	HANDLE hID;
	hID = reinterpret_cast<HANDLE>( ::_beginthreadex( NULL, 0, &WaveOut, this, 0, &uiID ) );
	::CloseHandle( hID );
}

int CNCWavOut::Decode( const char *p, const int& nSize )
{
	int nWrite = m_SWav.m_nWrite % BUFFER_MAX;
	m_SWav.m_nWrite++;

	int nBufSize = nSize / 2;
	for( int nCnt = 0; nCnt < nBufSize; nCnt++ )
	{
		float buf[ 2 ];
		for( int i = 0; i < 2; i++ )
		{
			int nPos = nCnt * 2 * 4 + i * 4;
			buf[ i ] = GetIEEE754( (const unsigned char*)&p[ nPos ] );
		}

		int a = (int)( buf[ 0 ] * 32768.f );
		int b = (int)( buf[ 1 ] * 32768.f );

		m_SWav.m_pData[ nWrite ][ nCnt * 4 + 1 ] = (char)( a >> 8 );
		m_SWav.m_pData[ nWrite ][ nCnt * 4 + 0 ] = (char)( a & 0x00FF );

		m_SWav.m_pData[ nWrite ][ nCnt * 4 + 3 ] = (char)( b >> 8 );
		m_SWav.m_pData[ nWrite ][ nCnt * 4 + 2 ] = (char)( b & 0x00FF );
	}

	m_SWav.m_WH[ nWrite ].lpData = (LPSTR)m_SWav.m_pData[ nWrite ];

	return 1;
}

}
