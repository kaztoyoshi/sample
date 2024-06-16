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
	static const int	CAMERA_WIDTH	= 160;									// ����
	static const int	CAMERA_HEIGHT	= 120;									// ����

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
	HMODULE			m_hModule;													// ���W���[��
	BOOL			m_bExec;													// ���s�t���O
	char			m_cNick[ NAME_MAX ];										// �j�b�N�l�[��
	int				m_nTimer;													// �Đ��J�E���g
	HWND			m_hOwn;														// �e�E�C���h�E�n���h��
	RECT			m_rcSize;													// �E�C���h�E�T�C�Y
	CV_INIT			cvInit;
	CV_PLAY			cvPlay;
	CV_ENCODE		cvEncode;
	CV_DECODE		cvDecode;
	CV_RELEASE		cvRelease;
};

}
#endif
