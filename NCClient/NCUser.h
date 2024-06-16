#ifndef __NCUSER_INCLUDE__
#define __NCUSER_INCLUDE__

#include "NCClient.h"
#include "NCAudio.h"

namespace	NetConfer
{
#pragma once
typedef int (WINAPI *CV_DDECODE)( const int& nW, const int& nH, unsigned char* mem_src, unsigned char* mem_dst );

class CNCUser
{
public:
	CNCUser( void );
	~CNCUser( void );

	void Init( const HDC& hdcWork, const int& nPosX, const int& nPosY );
	void Draw( void  );
	void End( void ){ m_CWavOut.End(); }

	BOOL Check( const char* cNick );
	void Video( const int& nTotal, const int& nStart, const int& nBlock, const BYTE byData[] );
	void Audio( const int& nTotal, const int& nStart, const int& nBlock, const BYTE byData[] );

private:
	BOOL			m_bUsed;													// �g�p�t���O
	HDC				m_hDC;														// �f�o�C�X�R���e�L�X�g
	int				m_nPosX;													// �\���w���W
	int				m_nPosY;													// �\���x���W
	char			m_szNickName[ NAME_MAX ];									// �j�b�N�l�[��
	BITMAPINFO		m_Bmi;														// BMP���
	HBITMAP			m_hBmp[ 2 ];												// BMP�n���h��
	LPBYTE			m_lpBuf[ 2 ];												// BMP�f�[�^
	int				m_nDispRead;												// �\���Ǎ��ؑ�
	int				m_nDispWrite;												// �\�������ؑ�
	BYTE*			m_pCamera;													// �J�����f�[�^
	HMODULE			m_hModule;													// ���W���[��
	CV_DDECODE		cvDDecode;
	CNCWavOut		m_CWavOut;													// �����Ǘ��N���X
	BYTE*			m_pAudio;													// �����f�[�^
};

}

#endif
