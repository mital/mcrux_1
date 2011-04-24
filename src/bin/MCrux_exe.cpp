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

#include <tchar.h>
#include <shlwapi.h>

#include <win32/stdafx.h>

#include <MCrux/Host.h>

std::string getPath()
{	
	char path[MAX_PATH]= { 0 };
	if ( GetModuleFileNameA( NULL, path, MAX_PATH))
		PathRemoveFileSpecA(path);
	return std::string(path);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);


	//string mcruxspec_path = "C:\\mcrux\\newapi\\window_create.mcruxspec";
	//string mcruxspec_path = "C:\\mcrux\\myapplication.mcruxspec";
	//string mcruxspec_path = "C:\\mcrux\\2Calculator\\calculator.mcruxspec";

	const std::string path(getPath());
	std::string mcruxspec_path(path);
	mcruxspec_path += "\\app.mcruxspec";
	std::string plugins_path(path);
	plugins_path += "\\plugins";

	Host* host = getHost();
	host->InitializeAndRunWith(mcruxspec_path, plugins_path);
	return 0;
}
