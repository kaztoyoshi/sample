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
	static const int	UDP_TIMEWAIT	= 10;									// 10ms�҂�
	static const int	UDP_BUFFMAX		= 64+(256*5)+64;						// UDP����M�T�C�Y(1408B)
	static const int	UDP_BUFFARY		= 128;									// UDP�z��
	static const int	BLOCK_MAX		= 256 * 5;								// �]���T�C�Y

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
	WSADATA			m_wsaData;													// WSADATA�\����
	int				m_nRes;														// �Ԓl
	BOOL			m_bEndFlag;													// �ؒf�t���O
	BOOL			m_bConn;													// �ڑ��t���O
	char			m_cAddr[ TEXT_MAX ];										// �A�h���X
	int				m_nPort;													// �|�[�g�ԍ�
	BYTE			m_byUDPRecv[ UDP_BUFFARY ][ UDP_BUFFMAX ];					// UDP��M�o�b�t�@
	int				m_nURRead;													// UDP��M�ǂݍ��݈ʒu�i��j
	int				m_nURWrite;													// UDP��M�������݈ʒu�i��j
	BYTE			m_byUDPSend[ UDP_BUFFARY ][ UDP_BUFFMAX ];					// UDP���M�o�b�t�@
	int				m_nUSRead;													// UDP���M�ǂݍ��݈ʒu�i��j
	int				m_nUSWrite;													// UDP���M�������݈ʒu�i��j
};

}

#endif
