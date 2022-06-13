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

		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}

		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	protected:
		CefRefPtr<CefBrowser> m_Browser;
		CefWindowHandle m_BrowserHandle;
		IMPLEMENT_REFCOUNTING(CEFCore);
	};
}