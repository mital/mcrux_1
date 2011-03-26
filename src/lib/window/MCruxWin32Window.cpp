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
#include "resource.h"

#include <win32/stdafx.h>
#include <tchar.h>

#include "MCruxWin32Window.h"

map<HWND, MCruxWin32Window *> MCruxWin32Window::mcruxWindows;


MCruxWin32Window::MCruxWin32Window(const MCruxWindowConfiguration * _config,
						 MCruxWindowManager * windowManager)
	: MCruxWindow(_config),
	  hInstance(GetModuleHandle(NULL)),
	  hWnd(NULL),
	  handler(NULL)
{
	hWnd = CreateWindow(MCruxWin32Window::getWindowClassName(),
		config->getWindowTitle().c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, config->getWidth(), config->getHeight(), NULL, NULL, hInstance, NULL);

	this->insertBrowser();

	MCruxWin32Window::mcruxWindows[hWnd] = this;
}


MCruxWin32Window::~MCruxWin32Window()
{
	if(MCruxWin32Window::mcruxWindows[hWnd])
	{
		MCruxWin32Window::mcruxWindows.erase(MCruxWin32Window::mcruxWindows.find(hWnd));
	}
	delete config;
}

void MCruxWin32Window::insertBrowser()
{
	// Create the single static handler class instance
    handler = new ClientHandler();
    handler->SetMainHwnd(this->hWnd);

	RECT rect;
    GetClientRect(hWnd, &rect);

    // Initialize window info to the defaults for a child window
	CefWindowInfo info;  
    info.SetAsChild(hWnd, rect);

    // Creat the new child child browser window
    bool bRes = CefBrowser::CreateBrowser(info, false,
        static_cast<CefRefPtr<CefHandler> >(handler),
		config->getURL());
}

int MCruxWin32Window::ShowWindow() const
{
	return ::ShowWindow(hWnd, SW_SHOW);
}


int MCruxWin32Window::HideWindow() const
{
	return ::ShowWindow(hWnd, SW_HIDE);
}


int MCruxWin32Window::UpdateWindow() const
{
	return ::UpdateWindow(hWnd);
}


void MCruxWin32Window::resize() const
{
	handler->resizeBrowser();
}


TCHAR * MCruxWin32Window::getWindowClassName()
{
	static TCHAR windowClassName[MAX_LOADSTRING] = _T("MCruxWindowClass");
	return windowClassName;
}


void MCruxWin32Window::initWindowClass(HINSTANCE hInstance)
{
	static bool bInit = false;
	if (!bInit)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= MCruxWin32Window::WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MCRUX_ICON));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= MCruxWin32Window::getWindowClassName();
		wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassEx(&wcex);

		bInit = true;
	}
}


void MCruxWin32Window::unInitWindowClass(HINSTANCE hInstance)
{
	UnregisterClass(MCruxWin32Window::getWindowClassName(), hInstance);
}


LRESULT CALLBACK MCruxWin32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_SIZE:
		if(MCruxWin32Window::mcruxWindows[hWnd])
		{
			MCruxWin32Window::mcruxWindows[hWnd]->resize();
		}
        break; 
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}