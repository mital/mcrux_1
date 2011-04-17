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
#include <mcrux/MCrux.h>
#include "MCruxSpecParser.h"
#include <src/lib/window/MCruxWindowManager.h>

#include <cef/cef.h>
#include <cef/cef_wrapper.h>
//#include <cef/transfer_utils.h>

#ifdef WIN32
#include "window/MCruxWin32Window.h"

#include "../win32/stdafx.h"
#include <commctrl.h>
#include <objbase.h>
#include <shlwapi.h>
#include <wininet.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif

static CefSettings settings;
static CefBrowserSettings browserDefaults;

MCrux::MCrux()
{
}


MCrux::~MCrux()
{
}

void MCrux::Initialize(
#ifndef WIN32
    int argc, char **argv
#endif
    )
{
#ifdef WIN32
  // Initialize Common controls
  INITCOMMONCONTROLSEX InitCtrlEx;

  InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrlEx.dwICC  = 0x00004000; //ICC_STANDARD_CLASSES;
  InitCommonControlsEx(&InitCtrlEx);

  MCruxWin32Window::initWindowClass(GetModuleHandle(NULL));

  // Init COM
  OleInitialize(NULL);
#endif
}

void MCrux::UnInitialize()
{
#ifdef WIN32
  // Shut down COM.
  OleUninitialize();
  MCruxWin32Window::unInitWindowClass(GetModuleHandle(NULL));
#endif
}

bool MCrux::InitializeAndRunWith(const string & mcruxspec_path,
	  const string & plugin_path
#ifndef WIN32
    , int argc, char **argv
#endif
    )
{
  bool bRet = false;
  Initialize(
#ifndef WIN32
    argc, argv
#endif
  );

  // parse the given configuration file
  MCruxSpecParser parser;
  parser.parse(mcruxspec_path);

  list<MCruxWindowConfiguration*> mcruxWindowConfigs;
  parser.getWindowConfigList(mcruxWindowConfigs);

#ifdef OS_WIN32
#define TEST_SINGLE_THREADED_MESSAGE_LOOP
#ifdef TEST_SINGLE_THREADED_MESSAGE_LOOP
  // Initialize the CEF with messages processed using the current application's
  // message loop.
  settings.multi_threaded_message_loop = false;
#else
  // Initialize the CEF with messages processed using a separate UI thread.
  settings.multi_threaded_message_loop = true;
#endif
#endif // OS_WIN32

  // Setting private plugin path.
  CefString str(plugin_path);
  settings.extra_plugin_paths = cef_string_list_alloc();
  cef_string_list_append(settings.extra_plugin_paths, str.GetStruct());

  browserDefaults.web_security_disabled = true;
//  CefInitialize(settings, browserDefaults);

  MCruxWindowManager windowManager(mcruxWindowConfigs);


  if(mcruxWindowConfigs.size())
  {
#ifdef WIN32
    HACCEL hAccelTable = NULL;// LoadAccelerators(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_blah));

    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
 #ifdef TEST_SINGLE_THREADED_MESSAGE_LOOP
    // Allow the CEF to do its message loop processing.
    CefDoMessageLoopWork();
#endif

	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    bRet = true;
#endif
  }
  else
  {
#ifdef OS_WIN32
    ::MessageBoxA(0, "mcruxspec file does not have any windows\n you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile", "error", MB_OK);
#elif OS_OSX
	  cerr << "error: mcruxspec file does not have any windows" << endl 
		<< "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
#else
	  cerr << "error: mcruxspec file does not have any windows" << endl
		<< "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
#endif
  }
  
  // Shut down the CEF
  //CefShutdown();

  UnInitialize();
  return bRet;
}
