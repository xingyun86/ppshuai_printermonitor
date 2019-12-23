// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlstr.h>
#include <commctrl.h>

#include <winspool.h>
#pragma comment(lib, "winspool.lib")

//显示在屏幕中央
__inline static void CenterWindowInScreen(HWND hWnd)
{
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	// Get workarea rect.
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
		0,              // Not used
		&rcScreen,    // Screen rect information
		0);             // Not used

	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//显示在父窗口中央
__inline static void CenterWindowInParent(HWND hWnd, HWND hParentWnd)
{
	RECT rcWindow = { 0 };
	RECT rcParent = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	GetWindowRect(hParentWnd, &rcParent);
	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcParent.right - rcParent.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcParent.bottom - rcParent.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}