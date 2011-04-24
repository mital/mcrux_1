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


#ifndef _MCRUX_H_
#define _MCRUX_H_

#include <string>

using namespace std;

#ifdef WIN32
#include <win32/MCruxExport.h>
#else
#define MCRUX_API
#endif


class MCRUX_API Host
{
	void Initialize(
#ifndef WIN32
		int argc, char **argv
#endif
			);

	void UnInitialize();

public:

	Host();
	~Host();

	bool InitializeAndRunWith(const string & mcruxspec_path,
		const string & plugin_path
#ifndef WIN32
		, int argc, char **argv
#endif
		);
};

#endif // _MCRUX_H_
