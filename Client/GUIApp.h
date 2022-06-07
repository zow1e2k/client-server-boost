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
			Client::ClientApp* client,
			CEF::CEFApp* cef,
			HINSTANCE* hInstance,
			int* nCmdShow
		);
		~GUIApp();

		Client::ClientApp* getClient();
		CEF::CEFApp* getCefApp();
		HINSTANCE* getInstance();
		int* getCmdShow();
		CEF::CEFCore* getCefCore();
		void setCefCore(CEF::CEFCore* cefCore);
		CefClient* getCefClientGUI();

	private:
		Client::ClientApp* clientGUI;
		CEF::CEFApp* cefAppGUI;
		HINSTANCE* instance;
		int* cmdShow;
		CEF::CEFCore* cefCoreGUI;
		CefClient* cefClientGUI;
	};
}
