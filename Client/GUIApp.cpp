#include "GUIApp.h"
#include "CEFApp.h"
#include <iostream>

namespace GUI {

	GUIApp::GUIApp(
		std::shared_ptr<Client::ClientApp> client,
		CefRefPtr<CEF::CEFApp> cef,
		HINSTANCE hInstance,
		int nCmdShow
	) {
		this->clientGUI = client;
		this->cefAppGUI = cef;
		this->instance = hInstance;
		this->cmdShow = nCmdShow;

		return;
	}

	GUIApp::~GUIApp() {
		return;
	}

	HINSTANCE GUIApp::getInstance()
	{
		return HINSTANCE();
	}

	int GUIApp::getCmdShow()
	{
		return 0;
	}

	CefRefPtr<CEF::CEFCore> GUIApp::getCefCore()
	{
		return this->cefCoreGUI;
	}

	void GUIApp::setCefCore(CefRefPtr<CEF::CEFCore> cefCore)
	{
		this->cefCoreGUI = cefCore;
	}

	CefRefPtr<CefClient> GUIApp::getCefClientGUI()
	{
		return this->cefClientGUI;
	}

	std::shared_ptr<Client::ClientApp> GUIApp::getClient()
	{
		return this->clientGUI;
	}

	CefRefPtr<CEF::CEFApp> GUIApp::getCefApp() {
		return this->cefAppGUI;
	}
}