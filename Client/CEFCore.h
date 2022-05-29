#pragma once

#include "include/cef_render_process_handler.h"
#include "include/cef_client.h"
#include "include/cef_v8.h"
#include "include/cef_browser.h"

namespace CEF {

	class CEFCore : public CefClient, public CefLifeSpanHandler {
	public:
		CEFCore();

		CefRefPtr<CefBrowser> GetBrowser() {
			return m_Browser;
		}

		CefWindowHandle GetBrowserHwnd() {
			return m_BrowserHandle;
		}

		// CefClient methods
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}

		// Virtual on CefLifeSpanHandler
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	protected:
		// The child browser window
		CefRefPtr<CefBrowser> m_Browser;

		// The child browser window handle
		CefWindowHandle m_BrowserHandle;

		// /
		// Macro that provides a reference counting implementation for classes
		// extending CefBase.
		// /
		IMPLEMENT_REFCOUNTING(CEFCore);
	};
}