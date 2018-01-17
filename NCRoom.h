#ifndef __NCROOM_INCLUDE__
#define __NCROOM_INCLUDE__

#include "NCClient.h"
#include "NCConnect.h"
#include "NCCamera.h"
#include "NCAudio.h"
#include "NCUser.h"

namespace	NetConfer
{
#pragma once
struct SData
{
	SData( void )
		: pData( NULL )
		, nTotal( 0 )
		, nStart( 0 )
		, nBlock( 0 )
		, bFlag( FALSE )
	{
	}

	BYTE*			pData;														// データ
	int				nTotal;														// 全体サイズ
	int				nStart;														// 開始位置
	int				nBlock;														// 転送サイズ
	BOOL			bFlag;														// フラグ
};

class CNCRoom : public WLWnd::CWLWnd
{
private:
	static const int	ID_TIMER		= 30001;								// タイマーＩＤ
	static const int	TIMEOUT_VALUE	= 10;									// タイマー( 1000 / 60 )
	static const int	USER_MAX		= 1;									// ユーザー総数
	static const int	UDP_BUFFMAX		= 64+(256*5)+64;						// UDP送受信サイズ(1408B)
	static const int	BLOCK_MAX		= 256 * 5;								// 転送サイズ
	static const int	ROOM_WIDTH		= 160;									// 会議室の横幅
	static const int	ROOM_HEIGHT		= 120;									// 会議室の高さ
	static const int	WINDOW_WIDTH	= ( ROOM_WIDTH * 3 );					// 全体の横幅
	static const int	WINDOW_HEIGHT	= ( ROOM_HEIGHT * 3 );					// 全体の高さ

	#define PROCNAME	"Net Conference - Client"								// プログラム名

private:
	virtual LRESULT WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT DummyProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return 0L; }
	virtual LRESULT DialogFunc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return 0L; }

	void Exec( void );
	void Recv( void );
	void Send( void );
	void DrawLine( void );

public:
	CNCRoom( void );
	~CNCRoom( void );

	BOOL Init( const HINSTANCE& hInstance, const char* cAddr, const int& nPort, const char* cNickName );
	void End( void );

private:
	WLGdi::CWLGdi	*m_pGdi;													// デバイスコンテキストクラス
	HWND			m_hWnd;														// ウインドウハンドル
	HDC				m_hdcWork;													// 作業用デバイスコンテキスト
	HPEN			m_hPen;														// ライン表示用
	CNCConnect		m_CCon;														// 接続管理クラス
	CNCCamera		m_CCap;														// カメラ管理クラス
	BOOL			m_bCamera;													// カメラ使用フラグ
	CNCWavIn		m_CWavIn;													// 音声管理クラス
	SData			m_SJpg;														// JPG構造体
	SData			m_SSnd;														// SND構造体
	char			m_cAddr[ TEXT_MAX ];										// IPｱﾄﾞﾚｽ
	int				m_nPort;													// ﾎﾟｰﾄ番号
	char			m_szNickName[ NAME_MAX ];									// ニックネーム
	CNCUser			m_CUser0;													// 自分
	CNCUser			m_CUser1;													// 他人１
	CNCUser			m_CUser2;													// 他人２
	CNCUser			m_CUser3;													// 他人３
	CNCUser			m_CUser4;													// 他人４
	CNCUser			m_CUser5;													// 他人５
	CNCUser			m_CUser6;													// 他人６
	CNCUser			m_CUser7;													// 他人７
	CNCUser			m_CUser8;													// 他人８
	CNCUser			*m_pUser[ USER_MAX ];										// ユーザー管理クラス配列
	CNCUser			*m_pHost;													// 主催者のアドレス
};

}
#endif
