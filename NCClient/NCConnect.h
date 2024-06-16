#ifndef __NCCONNECT_INCLUDE__
#define __NCCONNECT_INCLUDE__

#include "NCClient.h"
#include <process.h>

namespace	NetConfer
{
#pragma once
class CNCConnect
{
private:
	static const int	UDP_TIMEWAIT	= 10;									// 10ms待ち
	static const int	UDP_BUFFMAX		= 64+(256*5)+64;						// UDP送受信サイズ(1408B)
	static const int	UDP_BUFFARY		= 128;									// UDP配列数
	static const int	BLOCK_MAX		= 256 * 5;								// 転送サイズ

private:
	static unsigned __stdcall UDPConnect( void *pVoid );
public:
	CNCConnect( void );
	~CNCConnect( void );
	BOOL Start( const char* cAddr, const int& nPort );
	void End( void );
	void SetEndFlag( const BOOL& bFlag ){ m_bEndFlag = bFlag; }
	BOOL UDPRead( BYTE byData[] );
	void UDPWrite( const BYTE byData[] );

private:
	WSADATA			m_wsaData;													// WSADATA構造体
	int				m_nRes;														// 返値
	BOOL			m_bEndFlag;													// 切断フラグ
	BOOL			m_bConn;													// 接続フラグ
	char			m_cAddr[ TEXT_MAX ];										// アドレス
	int				m_nPort;													// ポート番号
	BYTE			m_byUDPRecv[ UDP_BUFFARY ][ UDP_BUFFMAX ];					// UDP受信バッファ
	int				m_nURRead;													// UDP受信読み込み位置（後）
	int				m_nURWrite;													// UDP受信書き込み位置（先）
	BYTE			m_byUDPSend[ UDP_BUFFARY ][ UDP_BUFFMAX ];					// UDP送信バッファ
	int				m_nUSRead;													// UDP送信読み込み位置（後）
	int				m_nUSWrite;													// UDP送信書き込み位置（先）
};

}

#endif
