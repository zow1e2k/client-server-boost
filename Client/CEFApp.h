
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

		int x;

		void OnWebKitInitialized() OVERRIDE;

		IMPLEMENT_REFCOUNTING(CEFApp);
	};
}