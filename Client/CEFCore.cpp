#include "CEFCore.h"

#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_web_plugin.h"

namespace CEF {

	CEFCore::CEFCore() {
	}

	bool CEFCore::DoClose(CefRefPtr<CefBrowser> browser) {
		return false;
	}

	void CEFCore::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		if (!m_Browser.get()) {
			// We need to keep the main child window, but not popup windows
			m_Browser = browser;
			m_BrowserHandle = browser->GetHost()->GetWindowHandle();
		}
	}

	void CEFCore::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		if (m_BrowserHandle == browser->GetHost()->GetWindowHandle()) {
			// Free the browser pointer so that the browser can be destroyed
			m_Browser = NULL;
		}
	}
}