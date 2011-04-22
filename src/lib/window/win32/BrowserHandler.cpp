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

#ifdef OS_WIN32
#include "Resource.h"
#include <tchar.h>
#endif

#include <sstream>
#include <list>

#include "BrowserHandler.h"

// ClientHandler implementation

ClientHandler::ClientHandler()
  : m_MainHwnd(NULL)
{
}

ClientHandler::~ClientHandler()
{
}

CefHandler::RetVal ClientHandler::HandleAfterCreated(
    CefRefPtr<CefBrowser> browser)
{
  REQUIRE_UI_THREAD();

  Lock();
  if(!browser->IsPopup())
  {
    // We need to keep the main child window, but not popup windows
    m_Browser = browser;
	m_BrowserHwnd = browser->GetWindowHandle();
  }
  Unlock();
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleLoadStart(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, bool isMainContent)
{
  REQUIRE_UI_THREAD();

  if(!browser->IsPopup() && !frame.get())
  {
    Lock();
    // We've just started loading a page
    Unlock();
  }
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleLoadEnd(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, bool isMainContent, int httpStatusCode)
{
  REQUIRE_UI_THREAD();

  if(!browser->IsPopup() && !frame.get())
  {
    Lock();
    // We've just finished loading a page
    Unlock();
  }
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleLoadError(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl,
    CefString& errorText)
{
  REQUIRE_UI_THREAD();

  if(errorCode == ERR_CACHE_MISS)
  {
    // Usually caused by navigating to a page with POST data via back or
    // forward buttons.
    errorText = "<html><head><title>Expired Form Data</title></head>"
                "<body><h1>Expired Form Data</h1>"
                "<h2>Your form request has expired. "
                "Click reload to re-submit the form data.</h2></body>"
                "</html>";
  }
  else
  {
    // All other messages.
    std::stringstream ss;
    ss <<       "<html><head><title>Load Failed</title></head>"
                "<body><h1>Load Failed</h1>"
                "<h2>Load of URL " << std::string(failedUrl) <<
                " failed with error code " << static_cast<int>(errorCode) <<
                ".</h2></body>"
                "</html>";
    errorText = ss.str();
  }
  return RV_HANDLED;
}

CefHandler::RetVal ClientHandler::HandleDownloadResponse(
    CefRefPtr<CefBrowser> browser, const CefString& mimeType,
    const CefString& fileName, int64 contentLength,
    CefRefPtr<CefDownloadHandler>& handler)
{
  REQUIRE_UI_THREAD();

  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandlePrintHeaderFooter(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    CefPrintInfo& printInfo, const CefString& url, const CefString& title,
    int currentPage, int maxPages, CefString& topLeft,
    CefString& topCenter, CefString& topRight, CefString& bottomLeft,
    CefString& bottomCenter, CefString& bottomRight)
{
  REQUIRE_UI_THREAD();

  // Place the page title at top left
  topLeft = title;
  // Place the page URL at top right
  topRight = url;
  
  // Place "Page X of Y" at bottom center
  std::stringstream strstream;
  strstream << "Page " << currentPage << " of " << maxPages;
  bottomCenter = strstream.str();

  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleJSBinding(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Value> object)
{
  REQUIRE_UI_THREAD();

  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleBeforeWindowClose(
    CefRefPtr<CefBrowser> browser)
{
  REQUIRE_UI_THREAD();
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleConsoleMessage(
    CefRefPtr<CefBrowser> browser, const CefString& message,
    const CefString& source, int line)
{
  REQUIRE_UI_THREAD();
  return RV_CONTINUE;
/*
  Lock();
  bool first_message = false;//m_LogFile.empty();
  if(first_message) {
    std::stringstream ss;
    ss << AppGetWorkingDirectory();
#ifdef _WIN32
    ss << "\\";
#else
    ss << "/";
#endif
    ss << "console.log";
    m_LogFile = ss.str();
  }
  std::string logFile = m_LogFile;
  Unlock();
  
  FILE* file = fopen(logFile.c_str(), "a");
  if(file) {
    std::stringstream ss;
    ss << "Message: " << std::string(message) << "\r\nSource: " <<
        std::string(source) << "\r\nLine: " << line <<
        "\r\n-----------------------\r\n";
    fputs(ss.str().c_str(), file);
    fclose(file);

    if(first_message)
      SendNotification(NOTIFY_CONSOLE_MESSAGE);
  }
*/
  return RV_CONTINUE;
}

void ClientHandler::GetNavState(bool &isLoading, bool &canGoBack,
                                bool &canGoForward)
{
}

void ClientHandler::SetMainHwnd(CefWindowHandle hwnd)
{
  Lock();
  m_MainHwnd = hwnd;
  Unlock();
}

////////////////////////////////////////////////////////
/////////////////////// win32 specifics ////////////////
////////////////////////////////////////////////////////

#ifdef OS_WIN32
// ClientHandler implementation

static void AddMenuItem(CefRefPtr<CefBrowser> browser, HMENU menu, int index,
                        CefHandler::MenuId id, const wchar_t* label,
                        bool enabled, std::list<std::wstring>& label_list)
{
  std::wstring actual_label = label;
  CefRefPtr<CefHandler> handler = browser->GetHandler();
  if(handler.get()) {
    // Let the handler change the label if desired
	  handler->HandleGetMenuLabel(browser, id, CefString(actual_label));
  }
  
  // store the label in a list to simplify memory management
  label_list.push_back(actual_label);
  
  MENUITEMINFO mii;
  mii.cbSize = sizeof(mii);
  mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
  mii.fType = MFT_STRING;
  if(!enabled) {
    mii.fMask |= MIIM_STATE;
    mii.fState = MFS_GRAYED;
  }
  mii.wID = id;
  mii.dwTypeData = const_cast<wchar_t*>(label_list.back().c_str());

  InsertMenuItem(menu, index, TRUE, &mii);
}


static void AddMenuSeparator(HMENU menu, int index)
{
  MENUITEMINFO mii;
  mii.cbSize = sizeof(mii);
  mii.fMask = MIIM_FTYPE;
  mii.fType = MFT_SEPARATOR;

  InsertMenuItem(menu, index, TRUE, &mii);
}
#endif

// Called on the UI thread before a context menu is displayed. To cancel
// display of the default context menu return RV_HANDLED.
CefHandler::RetVal ClientHandler::HandleBeforeMenu(CefRefPtr<CefBrowser> browser,
                                  const MenuInfo& menuInfo)
{
	REQUIRE_UI_THREAD();
#ifdef OS_WIN32
	HMENU menu = NULL;
	std::list<std::wstring> label_list;

	int type_flags = MENUTYPE_NONE;
	if(menuInfo.selectionText.length > 0)
		type_flags |= MENUTYPE_SELECTION;

	CefRefPtr<CefHandler> handler = browser->GetHandler();
	// Build the correct default context menu
	if (true) {
		menu = CreatePopupMenu();
		AddMenuItem(browser, menu, -1, MENU_ID_PRINT, L"Show Inspector",
			true, label_list);

	} else if (type_flags &  MENUTYPE_EDITABLE) {
		//menu = CreatePopupMenu();
		//AddMenuItem(browser, menu, -1, MENU_ID_UNDO, L"Undo",
		//	!!(edit_flags & WebContextMenuData::CanUndo), label_list);
		//AddMenuItem(browser, menu, -1, MENU_ID_REDO, L"Redo",
		//	!!(edit_flags & WebContextMenuData::CanRedo), label_list);
		//AddMenuSeparator(menu, -1);
		//AddMenuItem(browser, menu, -1, MENU_ID_CUT, L"Cut",
		//	!!(edit_flags & WebContextMenuData::CanCut), label_list);
		//AddMenuItem(browser, menu, -1, MENU_ID_COPY, L"Copy",
		//	!!(edit_flags & WebContextMenuData::CanCopy), label_list);
		//AddMenuItem(browser, menu, -1, MENU_ID_PASTE, L"Paste",
		//	!!(edit_flags & WebContextMenuData::CanPaste), label_list);
		//AddMenuItem(browser, menu, -1, MENU_ID_DELETE, L"Delete",
		//	!!(edit_flags & WebContextMenuData::CanDelete), label_list);
		//AddMenuSeparator(menu, -1);
		//AddMenuItem(browser, menu, -1, MENU_ID_SELECTALL, L"Select All",
		//	!!(edit_flags & MENU_CAN_SELECT_ALL), label_list);
	}
	//else if(type_flags & MENUTYPE_SELECTION) {
	//	menu = CreatePopupMenu();
	//	AddMenuItem(browser, menu, -1, MENU_ID_COPY, L"Copy",
	//		!!(edit_flags & WebContextMenuData::CanCopy), label_list);
	//} else if(type_flags & (MENUTYPE_PAGE | MENUTYPE_FRAME)) {
	//	menu = CreatePopupMenu();
	//	AddMenuItem(browser, menu, -1, MENU_ID_NAV_BACK, L"Back",
	//		browser->UIT_CanGoBack(), label_list);
	//	AddMenuItem(browser, menu, -1, MENU_ID_NAV_FORWARD, L"Forward",
	//		browser->UIT_CanGoForward(), label_list);
	//	AddMenuSeparator(menu, -1);
	//	AddMenuItem(browser, menu, -1, MENU_ID_PRINT, L"Print",
	//		true, label_list);
	//	AddMenuItem(browser, menu, -1, MENU_ID_VIEWSOURCE, L"View Source",
	//		true, label_list);
	//}

	if(menu) {
		// show the context menu
		int selected_id = TrackPopupMenu(menu,
			TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_RECURSE,
			menuInfo.x, menuInfo.y, 0, browser->GetWindowHandle(), NULL);

		if(selected_id != 0) {
			// An action was chosen
			CefHandler::MenuId menuId = static_cast<CefHandler::MenuId>(selected_id);
			bool handled = false;
			if(handler.get()) {
				// Ask the handler if it wants to handle the action
				CefHandler::RetVal rv = handler->HandleMenuAction(browser, menuId);
				handled = (rv == RV_HANDLED);
			}

			//if(!handled) {
			//	// Execute the action
			//	CefRefPtr<CefFrame> frame = browser->GetFocusedFrame();
			//	frame->AddRef();
			//	browser->Handle(menuId, frame.get());
			//}
		}
	}

	DestroyMenu(menu);

	return RV_HANDLED;
#else
	// TODO:
	return RV_CONTINUE;
#endif
}

CefHandler::RetVal ClientHandler::HandleMenuAction(CefRefPtr<CefBrowser> browser,
                              MenuId menuId)
{
	REQUIRE_UI_THREAD();
	if (menuId == MENU_ID_PRINT)
		browser->ShowDevTools();
	return RV_HANDLED;
}

CefHandler::RetVal ClientHandler::HandleBeforeCreated(
    CefRefPtr<CefBrowser> parentBrowser, CefWindowInfo& createInfo, bool popup,
    const CefPopupFeatures& popupFeatures, CefRefPtr<CefHandler>& handler,
    CefString& url, CefBrowserSettings& settings)
{
  REQUIRE_UI_THREAD();

  if(popup) {
    if(popupFeatures.xSet)
      createInfo.m_x = popupFeatures.x;
    if(popupFeatures.ySet)
      createInfo.m_y = popupFeatures.y;
    if(popupFeatures.widthSet)
      createInfo.m_nWidth = popupFeatures.width;
    if(popupFeatures.heightSet)
      createInfo.m_nHeight = popupFeatures.height;
  }

  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleAddressChange(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    const CefString& url)
{
  REQUIRE_UI_THREAD();
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleTitleChange(
    CefRefPtr<CefBrowser> browser, const CefString& title)
{
  REQUIRE_UI_THREAD();

  // Set the frame window title bar
/*  CefWindowHandle hwnd = browser->GetWindowHandle();
  if(!browser->IsPopup())
  {
    // The frame window will be the parent of the browser window
    hwnd = GetParent(hwnd);
  }
  SetWindowText(hwnd, std::wstring(title).c_str());*/
  return RV_CONTINUE;
}

CefHandler::RetVal ClientHandler::HandleBeforeResourceLoad(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefRequest> request,
    CefString& redirectUrl, CefRefPtr<CefStreamReader>& resourceStream,
    CefString& mimeType, int loadFlags)
{
  REQUIRE_IO_THREAD();
/*
  DWORD dwSize;
  LPBYTE pBytes;
  
  std::string url = request->GetURL();
  if(url == "http://tests/request") {
    // Show the request contents
    std::string dump;
    DumpRequestContents(request, dump);
    resourceStream =
        CefStreamReader::CreateForData((void*)dump.c_str(), dump.size());
    mimeType = "text/plain";
  } else if(url == "http://tests/uiapp") {
    // Show the uiapp contents
    if(LoadBinaryResource(IDS_UIPLUGIN, dwSize, pBytes)) {
      resourceStream = CefStreamReader::CreateForHandler(
          new CefByteReadHandler(pBytes, dwSize, NULL));
      mimeType = "text/html";
    }
  } else if(url == "http://tests/localstorage") {
    // Show the localstorage contents
    if(LoadBinaryResource(IDS_LOCALSTORAGE, dwSize, pBytes)) {
      resourceStream = CefStreamReader::CreateForHandler(
          new CefByteReadHandler(pBytes, dwSize, NULL));
      mimeType = "text/html";
    }
  } else if(url == "http://tests/xmlhttprequest") {
    // Show the xmlhttprequest HTML contents
    if(LoadBinaryResource(IDS_XMLHTTPREQUEST, dwSize, pBytes)) {
      resourceStream = CefStreamReader::CreateForHandler(
          new CefByteReadHandler(pBytes, dwSize, NULL));
      mimeType = "text/html";
    }
  } else if(strstr(url.c_str(), "/ps_logo2.png") != NULL) {
    // Any time we find "ps_logo2.png" in the URL substitute in our own image
    if(LoadBinaryResource(IDS_LOGO, dwSize, pBytes)) {
      resourceStream = CefStreamReader::CreateForHandler(
          new CefByteReadHandler(pBytes, dwSize, NULL));
      mimeType = "image/png";
    }
  } else if(strstr(url.c_str(), "/logoball.png") != NULL) {
    // Load the "logoball.png" image resource.
    if(LoadBinaryResource(IDS_LOGOBALL, dwSize, pBytes)) {
      resourceStream = CefStreamReader::CreateForHandler(
          new CefByteReadHandler(pBytes, dwSize, NULL));
      mimeType = "image/png";
    }
  }*/
  return RV_CONTINUE;
}

void ClientHandler::SendNotification(NotificationType type)
{
/*  UINT id;
  switch(type)
  {
  case NOTIFY_CONSOLE_MESSAGE:
    id = ID_WARN_CONSOLEMESSAGE;
    break;
  case NOTIFY_DOWNLOAD_COMPLETE:
    id = ID_WARN_DOWNLOADCOMPLETE;
    break;
  case NOTIFY_DOWNLOAD_ERROR:
    id = ID_WARN_DOWNLOADERROR;
    break;
  default:
    return;
  }
  PostMessage(m_MainHwnd, WM_COMMAND, id, 0);*/
}

void ClientHandler::resizeBrowser()
{
#ifdef OS_WIN32
    RECT rcClient;
	GetClientRect(this->m_MainHwnd, &rcClient);
	MoveWindow(m_BrowserHwnd, 0, 0, rcClient.right, rcClient.bottom, TRUE);
#else
	// TODO:
#endif

}