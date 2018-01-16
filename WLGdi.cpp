#include "WLGdi.h"

namespace	WLGdi
{
CWLGdi::CWLGdi( const HWND hWnd, const int nWidth, const int nHeight )
	: m_hWnd( hWnd )
	, m_hdcMain( NULL )
	, m_hdcWork( NULL )
	, m_nWidth( nWidth )
	, m_nHeight( nHeight )
{
}

CWLGdi::~CWLGdi( void )
{
	if( m_hdcWork != NULL )		::DeleteDC( m_hdcWork );
	if( m_hdcMain != NULL )		::DeleteDC( m_hdcMain );
}

BOOL CWLGdi::InitGDI( void )
{
	m_hdcMain = ::GetDC( m_hWnd );
	HBITMAP hBmpWork = ::CreateCompatibleBitmap( m_hdcMain, m_nWidth, m_nHeight );
	m_hdcWork = ::CreateCompatibleDC( m_hdcMain );
	::SelectObject( m_hdcWork, hBmpWork );
	::DeleteObject( hBmpWork );
	return EXIT_SUCCESS;
}

void CWLGdi::ClearGDI( const int nR, const int nG, const int nB )
{
	RECT		reRect;
	HBRUSH hBrush = ::CreateSolidBrush( RGB( nR, nG, nB ) );
	::SetRect( &reRect, 0, 0, m_nWidth, m_nHeight );
	::FillRect( m_hdcWork, &reRect, hBrush );
	::DeleteObject( hBrush );
}

void CWLGdi::DrawGDI( void )
{
	::BitBlt( m_hdcMain, 0, 0, m_nWidth, m_nHeight, m_hdcWork, 0, 0, SRCCOPY );
}

const HDC CWLGdi::GetWorkGDI( void ) const
{
	return m_hdcWork;
}

}
