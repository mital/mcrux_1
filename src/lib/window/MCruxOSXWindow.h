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

class MCruxWindowManager;

class MCruxOSXWindow : public MCruxWindow
{
	
public:
	MCruxOSXWindow(const MCruxWindowConfiguration * _config,
		MCruxWindowManager * windowManager);
	~MCruxOSXWindow();

	void insertBrowser();
	virtual int ShowWindow() const;
	virtual int HideWindow() const;
	virtual int UpdateWindow() const;
	virtual void resize() const;

private:
	CefRefPtr<ClientHandler> handler;

//	static map<HWND, MCruxOSXWindow *> mcruxWindows;

};

#endif // _MCRUXWIN32WINDOW_H_