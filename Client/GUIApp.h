#pragma once

//#include "include/cef_app.h"
//#include "include/cef_browser.h"
#include "CEFApp.h"
#include "CEFCore.h"
#include "ClientApp.h"

namespace GUI {

	class GUIApp : public std::enable_shared_from_this<GUIApp> {

	public:
		GUIApp(
			std::shared_ptr<Client::ClientApp> client,
			CefRefPtr<CEF::CEFApp> cef,
			HINSTANCE hInstance,
			int nCmdShow
		);
		~GUIApp();

		std::shared_ptr<Client::ClientApp> getClient();
		CefRefPtr<CEF::CEFApp> getCefApp();
		HINSTANCE getInstance();
		int getCmdShow();
		CefRefPtr<CEF::CEFCore> getCefCore();
		void setCefCore(CefRefPtr<CEF::CEFCore> cefCore);
		CefRefPtr<CefClient> getCefClientGUI();

	private:
		std::shared_ptr<Client::ClientApp> clientGUI;
		CefRefPtr<CEF::CEFApp> cefAppGUI;
		HINSTANCE instance;
		int cmdShow;
		CefRefPtr<CEF::CEFCore> cefCoreGUI;
		CefRefPtr<CefClient> cefClientGUI;
	};
}
