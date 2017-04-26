#pragma once
#include "BASE.h"

//
//
//
class WINDOW : public CELEMENT
{
public:
	//Data
	HWND m_hWnd;

	//Method
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
};