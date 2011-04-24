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


#ifndef _WIN32_HOST_H_
#define _WIN32_HOST_H_

#include <win32/MCruxExport.h>
#include <mcrux/Host.h>

#include <string>

using namespace std;

class MCRUX_API Win32Host : public Host
{
	virtual void Initialize();
	virtual void UnInitialize();
	virtual void RunLoop(const string & plugin_path);

public:

	Win32Host();
	virtual ~Win32Host();

	//bool InitializeAndRunWith(const string & mcruxspec_path,
	//	const string & plugin_path);
};

#endif // _WIN32_HOST_H_