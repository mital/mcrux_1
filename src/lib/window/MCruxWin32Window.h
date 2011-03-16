/**
 * copyright (C) 2009 Mital Vora. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of MCrux nor the names of its contributors may be 
 *     used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 *
 * @author: Mital Vora.
 **/

#ifndef _MCRUXWIN32WINDOW_H_
#define _MCRUXWIN32WINDOW_H_

#include "MCruxWindow.h"
#include "MCruxWindowConfiguration.h"
#include "BrowserHandler.h"

#ifdef WIN32
#include <windows.h>
#endif

class MCruxWindowManager;

class MCruxWin32Window : public MCruxWindow
{
private:
	HINSTANCE hInstance;
	HWND hWnd;
	CefRefPtr<ClientHandler> handler;

public:

	MCruxWin32Window(const MCruxWindowConfiguration * _config,
		MCruxWindowManager * windowManager);
	~MCruxWin32Window();

	void insertBrowser();
	virtual int ShowWindow() const;
	virtual int HideWindow() const;
	virtual int UpdateWindow() const;
	virtual void resize() const;

	static void initWindowClass(HINSTANCE hInstance);
	static void unInitWindowClass(HINSTANCE hInstance);

private:

	static TCHAR * getWindowClassName();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static map<HWND, MCruxWin32Window *> mcruxWindows;

};

#endif // _MCRUXWIN32WINDOW_H_