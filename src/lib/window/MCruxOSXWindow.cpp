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

#include "MCruxOSXWindow.h"

//map<HWND, MCruxOSXWindow *> MCruxOSXWindow::mcruxWindows;


MCruxOSXWindow::MCruxOSXWindow(const MCruxWindowConfiguration * _config,
						 MCruxWindowManager * windowManager)
	: MCruxWindow(_config),
	  handler(NULL)
{
	this->insertBrowser();

//	MCruxOSXWindow::mcruxWindows[hWnd] = this;
}


MCruxOSXWindow::~MCruxOSXWindow()
{
//	if(MCruxOSXWindow::mcruxWindows[hWnd])
	{
//		MCruxOSXWindow::mcruxWindows.erase(MCruxOSXWindow::mcruxWindows.find(hWnd));
	}
	delete config;
}

void MCruxOSXWindow::insertBrowser()
{
	// Create the single static handler class instance
//    handler = new ClientHandler();
//    handler->SetMainHwnd(this->hWnd);

    // Initialize window info to the defaults for a child window
	// CefWindowInfo info;  
    // info.SetAsChild(hWnd, rect);

    // Creat the new child child browser window
//    bool bRes = CefBrowser::CreateBrowser(info, false,
//        static_cast<CefRefPtr<CefHandler> >(handler),
//		config->getURL());
}

int MCruxOSXWindow::ShowWindow() const
{
	return 0;
}


int MCruxOSXWindow::HideWindow() const
{
	return 0;
}


int MCruxOSXWindow::UpdateWindow() const
{
	return 0;
}


void MCruxOSXWindow::resize() const
{
	handler->resizeBrowser();
}
