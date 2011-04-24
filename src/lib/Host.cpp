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

// MCrux.cpp : Defines the entry point for the DLL application.
//
#include <mcrux/Host.h>
#include "MCruxSpecParser.h"
#include <src/lib/window/MCruxWindowManager.h>

//#include "window/win32/MCruxWin32Window.h"
//
//#include "../win32/stdafx.h"
//#include <commctrl.h>
//#include <objbase.h>
//#include <shlwapi.h>
//#include <wininet.h>
//
//static CefSettings settings;
//static CefBrowserSettings browserDefaults;


Host::Host()
{
}


Host::~Host()
{
}

bool Host::InitializeAndRunWith(const string & mcruxspec_path,
								const string & plugin_path)
{
	bool bRet = false;
	Initialize();

	// parse the given configuration file
	MCruxSpecParser parser;
	parser.parse(mcruxspec_path);

	list<MCruxWindowConfiguration*> mcruxWindowConfigs;
	parser.getWindowConfigList(mcruxWindowConfigs);

	MCruxWindowManager windowManager(mcruxWindowConfigs);



	if(mcruxWindowConfigs.size())
	{
		this->RunLoop(plugin_path);
		bRet = true;
	}
	else
	{
#ifdef OS_WIN32
		cerr << "error: mcruxspec file does not have any windows" << endl 
			<< "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
//		::MessageBoxA(0, "mcruxspec file does not have any windows\n you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile", "error", MB_OK);
#elif OS_OSX
		cerr << "error: mcruxspec file does not have any windows" << endl 
			<< "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
#else
		cerr << "error: mcruxspec file does not have any windows" << endl
			<< "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
#endif
	}

	UnInitialize();
	return bRet;
}
