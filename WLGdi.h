#ifndef	__WLGDI_INCLUDE__
#define	__WLGDI_INCLUDE__

#include "WL.h"

using namespace std;

namespace	WLGdi
{
#pragma once

class CWLGdi
{
public:
	CWLGdi( const HWND hWnd, const int nWidth, const int nHeight );
	~CWLGdi( void );

	BOOL InitGDI( void );
	void ClearGDI( const int nR = 0xFF, const int nG = 0xFF, const int nB = 0xFF );
	void DrawGDI( void );
	const HDC GetWorkGDI( void ) const;

private:
	HWND				m_hWnd;													// ウインドウハンドル
	HDC					m_hdcMain;												// 表示用デバイスコンテキスト
	HDC					m_hdcWork;												// 作業用デバイスコンテキスト
	int					m_nWidth;												// 描画領域の横幅
	int					m_nHeight;												// 描画領域の高さ
};

}

#endif
