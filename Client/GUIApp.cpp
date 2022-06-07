#include "GUIApp.h"
#include "CEFApp.h"
#include <iostream>

namespace GUI {

	GUIApp::GUIApp(
		Client::ClientApp* client,
		CEF::CEFApp* cef,
		HINSTANCE* hInstance,
		int* nCmdShow
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

	HINSTANCE* GUIApp::getInstance()
	{
		return this->instance;
	}

	int* GUIApp::getCmdShow()
	{
		return this->cmdShow;
	}

	CEF::CEFCore* GUIApp::getCefCore()
	{
		return this->cefCoreGUI;
	}

	void GUIApp::setCefCore(CEF::CEFCore* cefCore)
	{
		this->cefCoreGUI = cefCore;
	}

	CefClient* GUIApp::getCefClientGUI()
	{
		return this->cefClientGUI;
	}

	Client::ClientApp* GUIApp::getClient()
	{
		return this->clientGUI;
	}

	CEF::CEFApp* GUIApp::getCefApp() {
		return this->cefAppGUI;
	}
}