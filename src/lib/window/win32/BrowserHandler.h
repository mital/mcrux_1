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

#ifndef _BROWSER_HANDLER_H_
#define _BROWSER_HANDLER_H_

#include <cef/cef.h>
#include <cef/cef_wrapper.h>


#ifdef OS_WIN32

#include <windows.h>

#ifdef _DEBUG
#define ASSERT(condition) if(!(condition)) { DebugBreak(); }
#else
#define ASSERT(condition) ((void)0)
#endif

// MSVC_PUSH_DISABLE_WARNING pushes |n| onto a stack of warnings to be disabled.
// The warning remains disabled until popped by MSVC_POP_WARNING.
#define MSVC_PUSH_DISABLE_WARNING(n) __pragma(warning(push)) \
                                     __pragma(warning(disable:n))

// MSVC_PUSH_WARNING_LEVEL pushes |n| as the global warning level.  The level
// remains in effect until popped by MSVC_POP_WARNING().  Use 0 to disable all
// warnings.
#define MSVC_PUSH_WARNING_LEVEL(n) __pragma(warning(push, n))

// Pop effects of innermost MSVC_PUSH_* macro.
#define MSVC_POP_WARNING() __pragma(warning(pop))

// Allows |this| to be passed as an argument in constructor initializer lists.
// This uses push/pop instead of the seemingly simpler suppress feature to avoid
// having the warning be disabled for more than just |code|.
//
// Example usage:
// Foo::Foo() : x(NULL), ALLOW_THIS_IN_INITIALIZER_LIST(y(this)), z(3) {}
//
// Compiler warning C4355: 'this': used in base member initializer list:
// http://msdn.microsoft.com/en-us/library/3c594ae3(VS.80).aspx
#define ALLOW_THIS_IN_INITIALIZER_LIST(code) MSVC_PUSH_DISABLE_WARNING(4355) \
                                             code \
                                             MSVC_POP_WARNING()

#else // !_WIN32

#include <assert.h>

#ifdef _DEBUG
#define ASSERT(condition) if(!(condition)) { assert(false); }
#else
#define ASSERT(condition) ((void)0)
#endif

#define ALLOW_THIS_IN_INITIALIZER_LIST(code) code

#endif // !_WIN32

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));


//#include <abstract/MCruxWindow.h>
//#include <window/MCruxWindowConfiguration.h>

class ClientHandler : public CefThreadSafeBase<CefHandler>
{
public:
  ClientHandler();
  ~ClientHandler();

  // Called on the UI thread before a new window is created. The |parentBrowser|
  // parameter will point to the parent browser window, if any. The |popup|
  // parameter will be true if the new window is a popup window, in which case
  // |popupFeatures| will contain information about the style of popup window
  // requested. If you create the window yourself you should populate the window
  // handle member of |createInfo| and return RV_HANDLED.  Otherwise, return
  // RV_CONTINUE and the framework will create the window.  By default, a newly
  // created window will recieve the same handler as the parent window.  To
  // change the handler for the new window modify the object that |handler|
  // points to.
  virtual RetVal HandleBeforeCreated(CefRefPtr<CefBrowser> parentBrowser,
                                     CefWindowInfo& createInfo, bool popup,
                                     const CefPopupFeatures& popupFeatures,
                                     CefRefPtr<CefHandler>& handler,
                                     CefString& url,
                                     CefBrowserSettings& settings);

  // Called on the UI thread after a new window is created. The return value is
  // currently ignored.
  virtual RetVal HandleAfterCreated(CefRefPtr<CefBrowser> browser);

  // Called on the UI thread when a frame's address has changed. The return
  // value is currently ignored.
  virtual RetVal HandleAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString& url);

  // Called on the UI thread when the page title changes. The return value is
  // currently ignored.
  virtual RetVal HandleTitleChange(CefRefPtr<CefBrowser> browser,
                                   const CefString& title);

  // Called on the UI thread before browser navigation. The client has an
  // opportunity to modify the |request| object if desired.  Return RV_HANDLED
  // to cancel navigation.
  virtual RetVal HandleBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    NavType navType, bool isRedirect)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when the browser begins loading a page. The |frame|
  // pointer will be empty if the event represents the overall load status and
  // not the load status for a particular frame. |isMainContent| will be true if
  // this load is for the main content area and not an iframe. This method may
  // not be called if the load request fails. The return value is currently
  // ignored.
  virtual RetVal HandleLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 bool isMainContent);

  // Called on the UI thread when the browser is done loading a page. The
  // |frame| pointer will be empty if the event represents the overall load
  // status and not the load status for a particular frame. |isMainContent| will
  // be true if this load is for the main content area and not an iframe. This
  // method will be called irrespective of whether the request completes
  // successfully. The return value is currently ignored.
  virtual RetVal HandleLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               bool isMainContent,
                               int httpStatusCode);

  // Called on the UI thread when the browser fails to load a resource.
  // |errorCode| is the error code number and |failedUrl| is the URL that failed
  // to load. To provide custom error text assign the text to |errorText| and
  // return RV_HANDLED.  Otherwise, return RV_CONTINUE for the default error
  // text.
  virtual RetVal HandleLoadError(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 ErrorCode errorCode,
                                 const CefString& failedUrl,
                                 CefString& errorText);

  // Called on the IO thread before a resource is loaded.  To allow the resource
  // to load normally return RV_CONTINUE. To redirect the resource to a new url
  // populate the |redirectUrl| value and return RV_CONTINUE.  To specify data
  // for the resource return a CefStream object in |resourceStream|, set
  // |mimeType| to the resource stream's mime type, and return RV_CONTINUE. To
  // cancel loading of the resource return RV_HANDLED. Any modifications to
  // |request| will be observed.  If the URL in |request| is changed and
  // |redirectUrl| is also set, the URL in |request| will be used.
  virtual RetVal HandleBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefRequest> request,
                                          CefString& redirectUrl,
                                          CefRefPtr<CefStreamReader>& resourceStream,
                                          CefString& mimeType,
                                          int loadFlags);

  // Called on the IO thread to handle requests for URLs with an unknown
  // protocol component. Return RV_HANDLED to indicate that the request should
  // succeed because it was externally handled. Set |allow_os_execution| to true
  // and return RV_CONTINUE to attempt execution via the registered OS protocol
  // handler, if any. If RV_CONTINUE is returned and either |allow_os_execution|
  // is false or OS protocol handler execution fails then the request will fail
  // with an error condition.
  // SECURITY WARNING: YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED
  // ON SCHEME, HOST OR OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.
  virtual RetVal HandleProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool* allow_os_execution)
  {
    REQUIRE_IO_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when a server indicates via the
  // 'Content-Disposition' header that a response represents a file to download.
  // |mimeType| is the mime type for the download, |fileName| is the suggested
  // target file name and |contentLength| is either the value of the
  // 'Content-Size' header or -1 if no size was provided. Set |handler| to the
  // CefDownloadHandler instance that will recieve the file contents. Return
  // RV_CONTINUE to download the file or RV_HANDLED to cancel the file download.
  virtual RetVal HandleDownloadResponse(CefRefPtr<CefBrowser> browser,
                                        const CefString& mimeType,
                                        const CefString& fileName,
                                        int64 contentLength,
                                        CefRefPtr<CefDownloadHandler>& handler);

  // Called on the IO thread when the browser needs credentials from the user.
  // |isProxy| indicates whether the host is a proxy server. |host| contains the
  // hostname and port number. Set |username| and |password| and return
  // RV_HANDLED to handle the request. Return RV_CONTINUE to cancel the request.
  virtual RetVal HandleAuthenticationRequest(CefRefPtr<CefBrowser> browser,
                                             bool isProxy,
                                             const CefString& host,
                                             const CefString& realm,
                                             const CefString& scheme,
                                             CefString& username,
                                             CefString& password)
  {
    REQUIRE_IO_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread before a context menu is displayed. To cancel
  // display of the default context menu return RV_HANDLED.
  virtual RetVal HandleBeforeMenu(CefRefPtr<CefBrowser> browser,
                                  const MenuInfo& menuInfo);

  // Called on the UI thread to optionally override the default text for a
  // context menu item. |label| contains the default text and may be modified to
  // substitute alternate text. The return value is currently ignored.
  virtual RetVal HandleGetMenuLabel(CefRefPtr<CefBrowser> browser,
                                    MenuId menuId, CefString& label)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when an option is selected from the default context
  // menu. Return RV_HANDLED to cancel default handling of the action.
  virtual RetVal HandleMenuAction(CefRefPtr<CefBrowser> browser,
                                  MenuId menuId);

  // Called on the UI thread to allow customization of standard print options
  // before the print dialog is displayed. |printOptions| allows specification
  // of paper size, orientation and margins. Note that the specified margins may
  // be adjusted if they are outside the range supported by the printer. All
  // units are in inches. Return RV_CONTINUE to display the default print
  // options or RV_HANDLED to display the modified |printOptions|.
  virtual RetVal HandlePrintOptions(CefRefPtr<CefBrowser> browser,
                                    CefPrintOptions& printOptions)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread to format print headers and footers. |printInfo|
  // contains platform-specific information about the printer context. |url| is
  // the URL if the currently printing page, |title| is the title of the
  // currently printing page, |currentPage| is the current page number and
  // |maxPages| is the total number of pages. Six default header locations are
  // provided by the implementation: top left, top center, top right, bottom
  // left, bottom center and bottom right. To use one of these default locations
  // just assign a string to the appropriate variable. To draw the header and
  // footer yourself return RV_HANDLED. Otherwise, populate the approprate
  // variables and return RV_CONTINUE.
  virtual RetVal HandlePrintHeaderFooter(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefPrintInfo& printInfo,
                                         const CefString& url,
                                         const CefString& title,
                                         int currentPage, int maxPages,
                                         CefString& topLeft,
                                         CefString& topCenter,
                                         CefString& topRight,
                                         CefString& bottomLeft,
                                         CefString& bottomCenter,
                                         CefString& bottomRight);

  // Called on the UI thread to run a JS alert message. Return RV_CONTINUE to
  // display the default alert or RV_HANDLED if you displayed a custom alert.
  virtual RetVal HandleJSAlert(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& message)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread to run a JS confirm request. Return RV_CONTINUE to
  // display the default alert or RV_HANDLED if you displayed a custom alert. If
  // you handled the alert set |retval| to true if the user accepted the
  // confirmation.
  virtual RetVal HandleJSConfirm(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const CefString& message, bool& retval)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread to run a JS prompt request. Return RV_CONTINUE to
  // display the default prompt or RV_HANDLED if you displayed a custom prompt.
  // If you handled the prompt set |retval| to true if the user accepted the
  // prompt and request and |result| to the resulting value.
  virtual RetVal HandleJSPrompt(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                const CefString& message,
                                const CefString& defaultValue,
                                bool& retval,
                                CefString& result)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread for adding values to a frame's JavaScript 'window'
  // object. The return value is currently ignored.
  virtual RetVal HandleJSBinding(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Value> object);

  // Called on the UI thread just before a window is closed. The return value is
  // currently ignored.
  virtual RetVal HandleBeforeWindowClose(CefRefPtr<CefBrowser> browser);

  // Called on the UI thread when the browser component is about to loose focus.
  // For instance, if focus was on the last HTML element and the user pressed
  // the TAB key. The return value is currently ignored.
  virtual RetVal HandleTakeFocus(CefRefPtr<CefBrowser> browser, bool reverse)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when the browser component is requesting focus.
  // |isWidget| will be true if the focus is requested for a child widget of the
  // browser window. Return RV_CONTINUE to allow the focus to be set or
  // RV_HANDLED to cancel setting the focus.
  virtual RetVal HandleSetFocus(CefRefPtr<CefBrowser> browser,
                                bool isWidget)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when the browser component receives a keyboard
  // event. |type| is the type of keyboard event, |code| is the windows scan-
  // code for the event, |modifiers| is a set of bit-flags describing any
  // pressed modifier keys and |isSystemKey| is true if Windows considers this a
  // 'system key' message (see
  // http://msdn.microsoft.com/en-us/library/ms646286(VS.85).aspx). Return
  // RV_HANDLED if the keyboard event was handled or RV_CONTINUE to allow the
  // browser component to handle the event.
  virtual RetVal HandleKeyEvent(CefRefPtr<CefBrowser> browser,
                                KeyEventType type,
                                int code,
                                int modifiers,
                                bool isSystemKey)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when the browser is about to display a tooltip.
  // |text| contains the text that will be displayed in the tooltip. To handle
  // the display of the tooltip yourself return RV_HANDLED. Otherwise, you can
  // optionally modify |text| and then return RV_CONTINUE to allow the browser
  // to display the tooltip.
  virtual RetVal HandleTooltip(CefRefPtr<CefBrowser> browser,
                               CefString& text)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread when the browser has a status message. |text|
  // contains the text that will be displayed in the status message and |type|
  // indicates the status message type. The return value is currently ignored.
  virtual RetVal HandleStatus(CefRefPtr<CefBrowser> browser,
                              const CefString& text, StatusType type)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Called on the UI thread to display a console message. Return RV_HANDLED to
  // stop the message from being output to the console.
  virtual RetVal HandleConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      const CefString& message,
                                      const CefString& source, int line);

  // Called on the UI thread to report find results returned by
  // CefBrowser::Find(). |identifer| is the identifier passed to
  // CefBrowser::Find(), |count| is the number of matches currently identified,
  // |selectionRect| is the location of where the match was found (in window
  // coordinates), |activeMatchOrdinal| is the current position in the search
  // results, and |finalUpdate| is true if this is the last find notification.
  // The return value is currently ignored.
  virtual RetVal HandleFindResult(CefRefPtr<CefBrowser> browser,
                                  int identifier, int count,
                                  const CefRect& selectionRect,
                                  int activeMatchOrdinal, bool finalUpdate)
  {
    REQUIRE_UI_THREAD();
    return RV_CONTINUE;
  }

  // Retrieve the current navigation state flags
  void GetNavState(bool &isLoading, bool &canGoBack, bool &canGoForward);
  void SetMainHwnd(CefWindowHandle hwnd);
  CefWindowHandle GetMainHwnd() { return m_MainHwnd; }
  void SetEditHwnd(CefWindowHandle hwnd);
  CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }

  std::string GetLogFile();

  void SetLastDownloadFile(const std::string& fileName);
  std::string GetLastDownloadFile();

  // Send a notification to the application. Notifications should not block the
  // caller.
  enum NotificationType
  {
    NOTIFY_CONSOLE_MESSAGE,
    NOTIFY_DOWNLOAD_COMPLETE,
    NOTIFY_DOWNLOAD_ERROR,
  };
  void SendNotification(NotificationType type);

  void resizeBrowser();

protected:
  // The child browser window
  CefRefPtr<CefBrowser> m_Browser;

  // The main frame window handle
  CefWindowHandle m_MainHwnd;

  // The child browser window handle
  CefWindowHandle m_BrowserHwnd;

};



#endif // _BROWSER_HANDLER_H_