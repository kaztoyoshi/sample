#ifndef	__WLWND_INCLUDE__
#define	__WLWND_INCLUDE__

#include "WL.h"

using namespace std;

namespace	WLWnd
{

#pragma once
static const int	WINDOWCHAR			= 64;									// 文字定数

class CWLWnd
{
private:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

public:
	CWLWnd( void );
	~CWLWnd( void );

	BOOL InitWindow( const HINSTANCE hInstance, const int nState = SW_HIDE, const bool bExFlag = false );
	void SetWindowName( const LPSTR lpName );
	void SetWindowTitle( const LPSTR lpTitle );
	void SetWindowStyle( const DWORD dwStyle );
	void SetWindowXYWH( const int nX, const int nY, const int nW, const int nH );
	void SetBackground( const int nR = 0xFF, const int nG = 0xFF, const int nB = 0xFF );
	void SetShowWindow( const int nState );
	void SetEndFlag( const BOOL bFlag );
	const HWND GetWindowHandle( void ) const;
	const BOOL GetEndFlag( void ) const;
	void RenewalWindow( void );
	void RenewalWindow( const int nX, const int nY, const int nW, const int nH, const BOOL bFlag = TRUE );

private:
	HANDLE				m_hMutex;												// 多重起動防止ハンドル
	HWND				m_hWnd;													// ウインドウハンドル
	HWND				m_hOwnWnd;												// 親ウインドウハンドル
	HINSTANCE			m_hInst;												// インスタンスハンドル
	HCURSOR				m_hCur;													// カーソルハンドル
	HICON				m_hIcon;												// アイコンハンドル
	int					m_nBackground;											// 背景色
	BOOL				m_bEndFlag;												// 終了フラグ
	int					m_nWndSizeX, m_nWndSizeY;								// ローカルウインドウの大きさ
	int					m_nWndPosX, m_nWndPosY;									// ローカルウインドウの位置
	DWORD				m_dwWndStyle;											// ウインドウのスタイル
	char				m_szWndName[ WINDOWCHAR ];								// ウインドウの名前
	char				m_szWndTitle[ WINDOWCHAR ];								// タイトルの名前
	char				m_szWndMenu[ WINDOWCHAR ];								// メニューの名前
	char				m_szWndCursor[ WINDOWCHAR ];							// カーソルの名前
	char				m_szWndIcon[ WINDOWCHAR ];								// アイコンの名前
};

}

#endif
