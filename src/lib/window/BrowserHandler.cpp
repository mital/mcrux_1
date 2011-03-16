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
#include "Resource.h"
#include <tchar.h>
#include <sstream>

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

// ClientHandler implementation

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
    RECT rcClient;
	GetClientRect(this->m_MainHwnd, &rcClient);
	MoveWindow(m_BrowserHwnd, 0, 0, rcClient.right, rcClient.bottom, TRUE);

}