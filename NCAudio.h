#ifndef __NCAUDIO_INCLUDE__
#define __NCAUDIO_INCLUDE__

#include "NCClient.h"
#include <mmsystem.h>
#include <process.h>

#pragma comment( lib, "winmm.lib" )

namespace	NetConfer
{

#pragma once
#define BUFFER_MAX		2

struct SWav
{
	SWav( void )
		: m_HWI( NULL )
		, m_dwSecond( 1 )
		, m_nWrite( 0 )
		, m_nRead( 0 )
		, m_bState( FALSE )
		, m_bFlag( FALSE )
	{
		m_WFEX.nChannels		= 2/2;
		m_WFEX.wFormatTag		= WAVE_FORMAT_PCM;
		m_WFEX.wBitsPerSample	= 16;
		m_WFEX.nBlockAlign		= m_WFEX.nChannels * m_WFEX.wBitsPerSample / 8;
		m_WFEX.nSamplesPerSec	= 44100/2;
		m_WFEX.nAvgBytesPerSec	= m_WFEX.nSamplesPerSec * m_WFEX.nBlockAlign;
		m_WFEX.cbSize			= 0;
		for( int i = 0; i < BUFFER_MAX; i++ )
			m_pData[ i ] = NULL;
	}

	WAVEHDR			m_WH[ BUFFER_MAX ];
	HWAVEIN			m_HWI;
	HWAVEOUT		m_HWO;
	WAVEFORMATEX	m_WFEX;
	DWORD			m_dwSecond;
	char			*m_pData[ BUFFER_MAX ];
	int				m_nWrite;
	int				m_nRead;
	BOOL			m_bState;
	BOOL			m_bFlag;
};
class CNCWavIn
{
private:
	static void CALLBACK waveInProc( HWAVEIN hwi, UINT uMsg, DWORD dwInst, DWORD dwParam1, DWORD dwParam2 );
	static unsigned __stdcall WaveIn( void *pVoid );
public:
	CNCWavIn( void );
	~CNCWavIn( void );
	void Init( void );
	BOOL CheckBuf( void ){ return ( m_nRead < m_nWrite ) ? TRUE : FALSE; }
	BOOL CheckFlag( void ){ return m_SWav.m_bState; }
	int Encode( char *p );
	void End( void ){ m_SWav.m_bState = FALSE; }

private:
	SWav			m_SWav;														// 入力
	float			*m_pBuf[ BUFFER_MAX ][ 2 ];									// 転送データ
	int				m_nBufSize;													// バッファサイズ
	int				m_nWrite;													// 書込カウント
	int				m_nRead;													// 読込カウント
};

class CNCWavOut
{
private:
	static unsigned __stdcall WaveOut( void *pVoid );
	float GetIEEE754( const unsigned char *buf );
public:
	CNCWavOut( void );
	~CNCWavOut( void );

	void Init( void );
	int GetSize( void ){ return m_SWav.m_WFEX.nAvgBytesPerSec * m_SWav.m_dwSecond; }
	BOOL CheckFlag( void ){ return m_SWav.m_bState; }
	int Decode( const char *p, const int& nSize );
	void End( void ){ m_SWav.m_bState = FALSE; }

private:
	SWav			m_SWav;														// 出力
};

}
#endif
