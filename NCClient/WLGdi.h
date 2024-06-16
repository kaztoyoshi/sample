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
	HWND				m_hWnd;													// �E�C���h�E�n���h��
	HDC					m_hdcMain;												// �\���p�f�o�C�X�R���e�L�X�g
	HDC					m_hdcWork;												// ��Ɨp�f�o�C�X�R���e�L�X�g
	int					m_nWidth;												// �`��̈�̉���
	int					m_nHeight;												// �`��̈�̍���
};

}

#endif
