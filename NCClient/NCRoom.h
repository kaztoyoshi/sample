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

	BYTE*			pData;														// �f�[�^
	int				nTotal;														// �S�̃T�C�Y
	int				nStart;														// �J�n�ʒu
	int				nBlock;														// �]���T�C�Y
	BOOL			bFlag;														// �t���O
};

class CNCRoom : public WLWnd::CWLWnd
{
private:
	static const int	ID_TIMER		= 30001;								// �^�C�}�[�h�c
	static const int	TIMEOUT_VALUE	= 10;									// �^�C�}�[( 1000 / 60 )
	static const int	USER_MAX		= 1;									// ���[�U�[����
	static const int	UDP_BUFFMAX		= 64+(256*5)+64;						// UDP����M�T�C�Y(1408B)
	static const int	BLOCK_MAX		= 256 * 5;								// �]���T�C�Y
	static const int	ROOM_WIDTH		= 160;									// ��c���̉���
	static const int	ROOM_HEIGHT		= 120;									// ��c���̍���
	static const int	WINDOW_WIDTH	= ( ROOM_WIDTH * 3 );					// �S�̂̉���
	static const int	WINDOW_HEIGHT	= ( ROOM_HEIGHT * 3 );					// �S�̂̍���

	#define PROCNAME	"Net Conference - Client"								// �v���O������

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
	WLGdi::CWLGdi	*m_pGdi;													// �f�o�C�X�R���e�L�X�g�N���X
	HWND			m_hWnd;														// �E�C���h�E�n���h��
	HDC				m_hdcWork;													// ��Ɨp�f�o�C�X�R���e�L�X�g
	HPEN			m_hPen;														// ���C���\���p
	CNCConnect		m_CCon;														// �ڑ��Ǘ��N���X
	CNCCamera		m_CCap;														// �J�����Ǘ��N���X
	BOOL			m_bCamera;													// �J�����g�p�t���O
	CNCWavIn		m_CWavIn;													// �����Ǘ��N���X
	SData			m_SJpg;														// JPG�\����
	SData			m_SSnd;														// SND�\����
	char			m_cAddr[ TEXT_MAX ];										// IP���ڽ
	int				m_nPort;													// �߰Ĕԍ�
	char			m_szNickName[ NAME_MAX ];									// �j�b�N�l�[��
	CNCUser			m_CUser0;													// ����
	CNCUser			m_CUser1;													// ���l�P
	CNCUser			m_CUser2;													// ���l�Q
	CNCUser			m_CUser3;													// ���l�R
	CNCUser			m_CUser4;													// ���l�S
	CNCUser			m_CUser5;													// ���l�T
	CNCUser			m_CUser6;													// ���l�U
	CNCUser			m_CUser7;													// ���l�V
	CNCUser			m_CUser8;													// ���l�W
	CNCUser			*m_pUser[ USER_MAX ];										// ���[�U�[�Ǘ��N���X�z��
	CNCUser			*m_pHost;													// ��Î҂̃A�h���X
};

}
#endif
