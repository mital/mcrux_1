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

#ifdef WIN32
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


#else // for linux
#include <gtk/gtk.h>
#endif

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

  // Init COM
  OleInitialize(NULL);
#else
  gtk_init (&argc, &argv);
  if (!g_thread_supported ())
    g_thread_init (NULL);

#endif
}

void MCrux::UnInitialize()
{
#ifdef WIN32
  // Shut down COM.
  OleUninitialize();
#endif
}



bool MCrux::InitializeAndRunWith(const string & mcruxAppConfigFileName
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
  parser.parse(mcruxAppConfigFileName);

  list<MCruxWindowConfiguration*> mcruxWindowConfigs;
  parser.getWindowConfigList(mcruxWindowConfigs);

  if(mcruxWindowConfigs.size())
  {
#ifdef WIN32
    HACCEL hAccelTable = NULL;// LoadAccelerators(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_blah));

    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    bRet = true;
#else // for linux
    gtk_main();

#endif
  }
  else
  {
#ifdef WIN32
    ::MessageBoxA(0, "mcruxspec file does not have any windows\n you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile", "error", MB_OK);
#else
    cout << "error: mcruxspec file does not have any windows" << endl 
      << "you can refer documentation at http://code.google.com/p/mcrux/wiki/MCruxSpecFile" << endl;
#endif
  }

  UnInitialize();
  return bRet;
}
