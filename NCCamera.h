#ifndef __NCCAMERA_INCLUDE__
#define __NCCAMERA_INCLUDE__

#include "NCClient.h"
#include <process.h>

namespace	NetConfer
{

#pragma once

typedef BOOL (WINAPI *CV_INIT)( const RECT& rc, const char* cName, const HWND& hOwn );
typedef BOOL (WINAPI *CV_PLAY)( void );
typedef int (WINAPI *CV_DECODE)( unsigned char* mem_src, unsigned char* mem_dst );
typedef int (WINAPI *CV_ENCODE)( unsigned char* p );
typedef BOOL (WINAPI *CV_RELEASE)( void );

class CNCCamera
{
private:
	static const int	TIMER_COUNT		= 10;
public:
	static const int	CAMERA_WIDTH	= 160;									// 横幅
	static const int	CAMERA_HEIGHT	= 120;									// 高さ

private:
	static unsigned __stdcall InitThread( void *pVoid );
public:
	CNCCamera( void );
	~CNCCamera( void );

	BOOL Init( const char* cNick, const int& nX, const int& nY, const HWND& hOwn = NULL );
	BOOL Play( void );
	int Encode( unsigned char* p );
	int Decode( unsigned char* src, unsigned char* dst );
	BOOL Release( void );

private:
	HMODULE			m_hModule;													// モジュール
	BOOL			m_bExec;													// 実行フラグ
	char			m_cNick[ NAME_MAX ];										// ニックネーム
	int				m_nTimer;													// 再生カウント
	HWND			m_hOwn;														// 親ウインドウハンドル
	RECT			m_rcSize;													// ウインドウサイズ
	CV_INIT			cvInit;
	CV_PLAY			cvPlay;
	CV_ENCODE		cvEncode;
	CV_DECODE		cvDecode;
	CV_RELEASE		cvRelease;
};

}
#endif
