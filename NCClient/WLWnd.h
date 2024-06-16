#ifndef	__WLWND_INCLUDE__
#define	__WLWND_INCLUDE__

#include "WL.h"

using namespace std;

namespace	WLWnd
{

#pragma once
static const int	WINDOWCHAR			= 64;									// �����萔

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
	HANDLE				m_hMutex;												// ���d�N���h�~�n���h��
	HWND				m_hWnd;													// �E�C���h�E�n���h��
	HWND				m_hOwnWnd;												// �e�E�C���h�E�n���h��
	HINSTANCE			m_hInst;												// �C���X�^���X�n���h��
	HCURSOR				m_hCur;													// �J�[�\���n���h��
	HICON				m_hIcon;												// �A�C�R���n���h��
	int					m_nBackground;											// �w�i�F
	BOOL				m_bEndFlag;												// �I���t���O
	int					m_nWndSizeX, m_nWndSizeY;								// ���[�J���E�C���h�E�̑傫��
	int					m_nWndPosX, m_nWndPosY;									// ���[�J���E�C���h�E�̈ʒu
	DWORD				m_dwWndStyle;											// �E�C���h�E�̃X�^�C��
	char				m_szWndName[ WINDOWCHAR ];								// �E�C���h�E�̖��O
	char				m_szWndTitle[ WINDOWCHAR ];								// �^�C�g���̖��O
	char				m_szWndMenu[ WINDOWCHAR ];								// ���j���[�̖��O
	char				m_szWndCursor[ WINDOWCHAR ];							// �J�[�\���̖��O
	char				m_szWndIcon[ WINDOWCHAR ];								// �A�C�R���̖��O
};

}

#endif
