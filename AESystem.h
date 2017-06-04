// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AESystem.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the system module
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_SYSTEM_H
#define AE_SYSTEM_H

#include "AEExport.h"
#include "AETypes.h"

// ---------------------------------------------------------------------------

// window related variables
#if(EXPORT_WINDOWS == 1)
extern HWND			gAESysWindowHandle;
#endif

// ---------------------------------------------------------------------------



typedef struct AESysInitInfo
{
#if(EXPORT_WINDOWS == 1)
	int				mCreateWindow;
	HWND			mWindowHandle;
	HINSTANCE		mAppInstance;
	int				mShow;
	int				mWinWidth, mWinHeight;
	int				mCreateConsole;
	unsigned int	mMaxFrameRate;
	LRESULT (CALLBACK *mpWinCallBack)(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);			// NULL to use the default message handler

	unsigned int	mClassStyle;															
	unsigned int	mWindowStyle;															



#else
	int dummy;

#endif
}AESysInitInfo;


// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

#if(EXPORT_WINDOWS == 1)
AE_API int AESysInit				(AESysInitInfo *pSysInitInfo);//HINSTANCE hAppInstance, int show, int WinWidth, int WinHeight, int CreateConsole, unsigned int FrameRateMax, LRESULT (CALLBACK *pWinCallBack)(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp));
AE_API void AESysReset				();
AE_API void AESysUpdate				();
AE_API void AESysExit				();


AE_API HWND AESysGetWindowHandle	();
//AE_API int* AESysGetAppActive		();
AE_API void AESysSetWindowTitle		(const char *pTitle);
AE_API int AESysDoesWindowExist		();


// ---------------------------------------------------------------------------

AE_API void AESysFrameStart();
AE_API void AESysFrameEnd();

// ---------------------------------------------------------------------------


AE_API void AESysAddCustomCallBack(u32 MsgID, LRESULT (*pCallBack)(HWND hWin, UINT msg, WPARAM wp, LPARAM lp));
AE_API void AESysRemoveCustomCallBack(u32 MsgID, LRESULT (*pCallBack)(HWND hWin, UINT msg, WPARAM wp, LPARAM lp));
AE_API void AESysClearCustomCallBacks();
#endif

#ifdef __cplusplus 
}
#endif

#endif // AE_SYSTEM_H

