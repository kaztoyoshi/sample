#include "NCClient.h"
#include "NCRoom.h"

char	g_szNickName[ NAME_MAX ];												// ニックネーム

LRESULT CALLBACK DialogProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		::SetDlgItemText( hWnd, IDC_EDIT1, "名前登録" );
		return FALSE;
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDC_BUTTON1:
			::GetDlgItemText( hWnd, IDC_EDIT1, g_szNickName, MAX_PATH );
			if( ::strcmp( g_szNickName, "" ) == 0 )
				return FALSE;
			::EndDialog( hWnd, TRUE );
			break;
		case IDC_BUTTON2:
			::EndDialog( hWnd, FALSE );
			break;
		default: return FALSE;
		}
	default: return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	MSG		msg;
	NetConfer::CNCRoom Room;
	const char* cAddr = "227.192.1.2";
	const int nPort = 44800;

	if( !::DialogBox( hInstance, "DIALOG", NULL, reinterpret_cast<DLGPROC>( DialogProc ) ) )
		goto PROC_EXIT;

	if( Room.Init( hInstance, cAddr, nPort, g_szNickName ) == EXIT_FAILURE )
		goto PROC_EXIT;

	while( 1 )
	{
		if( Room.GetEndFlag() ) break;
		while( ::GetMessage( &msg, NULL, 0, 0 ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}
	Room.End();
PROC_EXIT:

	return EXIT_SUCCESS;
}