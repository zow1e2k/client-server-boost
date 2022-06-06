
#pragma once
#include "include/cef_app.h"
#include "include/cef_client.h"

namespace CEF {

	class CEFApp : public CefApp, public CefRenderProcessHandler {
	public:
		CEFApp();

		CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
			return this;
		}

		//void OnWebKitInitialized() OVERRIDE;
		void OnContextCreated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context
		) OVERRIDE;
		bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message
		) OVERRIDE;

		IMPLEMENT_REFCOUNTING(CEFApp);
	};
}