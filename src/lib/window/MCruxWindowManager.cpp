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
#include "MCruxWindowManager.h"

#ifdef WIN32
#include "win32/MCruxWin32Window.h"
#elif OS_OSX
#include "MCruxOSXWindow.h"
#endif

MCruxWindowManager::MCruxWindowManager(const list<MCruxWindowConfiguration*> & _windowConfigurations)
{
	if(_windowConfigurations.size())
	{
		for(list<MCruxWindowConfiguration*>::const_iterator
			oIter = _windowConfigurations.begin();
			oIter != _windowConfigurations.end();
		oIter++)
		{
			MCruxWindowConfiguration * mainWindowConfig = *oIter;
      MCruxWindow * newWindow = NULL;

#ifdef OS_WIN32
			newWindow = new MCruxWin32Window(mainWindowConfig, this);
#elif OS_OSX
			newWindow = new MCruxOSXWindow(mainWindowConfig, this);
#endif

			newWindow->ShowWindow();
			newWindow->UpdateWindow();
			windows.push_back(newWindow);
		}
	}
}


MCruxWindowManager::~MCruxWindowManager()
{
}