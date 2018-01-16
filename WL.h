#ifndef	__WL_INCLUDE__
#define	__WL_INCLUDE__

#define	WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <shlwapi.h>
#include <direct.h>

#include <vector>

#include "WLWnd.h"
#include "WLGdi.h"

#pragma comment( lib, "shlwapi.lib" )


#ifdef	_DEBUG
	#define	ASSERT( flag )	assert( flag );
#else
	#define	ASSERT( flag )	{}
#endif

#define EXIT				1
#define TRUE				1
#define FALSE				0

#endif
