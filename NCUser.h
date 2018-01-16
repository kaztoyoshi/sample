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
	BOOL			m_bUsed;													// 使用フラグ
	HDC				m_hDC;														// デバイスコンテキスト
	int				m_nPosX;													// 表示Ｘ座標
	int				m_nPosY;													// 表示Ｙ座標
	char			m_szNickName[ NAME_MAX ];									// ニックネーム
	BITMAPINFO		m_Bmi;														// BMP情報
	HBITMAP			m_hBmp[ 2 ];												// BMPハンドル
	LPBYTE			m_lpBuf[ 2 ];												// BMPデータ
	int				m_nDispRead;												// 表示読込切替
	int				m_nDispWrite;												// 表示書込切替
	BYTE*			m_pCamera;													// カメラデータ
	HMODULE			m_hModule;													// モジュール
	CV_DDECODE		cvDDecode;
	CNCWavOut		m_CWavOut;													// 音声管理クラス
	BYTE*			m_pAudio;													// 音声データ
};

}

#endif
