#pragma once

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "CEFCore.h"
#include "ClientCore.h"

namespace Client {
	using namespace CEF;

	class GUIApp {

	public:
		static std::string GetApplicationDir();
		static HWND RegisterWindow(HINSTANCE hInstance, int nCmdShow);
		static LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static HWND CreateMessageWindow(HINSTANCE hInstance);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static int create(HINSTANCE hInstance, int nCmdShow);
		GUIApp();
		void setCefCore(CEFCore* cefCore);
		int initCef(HINSTANCE hInstance, int nCmdShow);

	private:
		CEFCore* cefCore_;
	};
}
