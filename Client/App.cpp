#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <string>
#include <algorithm>
//#include "GUIApp.h"
#include "ClientApp.h"
#include "CEFApp.h"
#include "GUIApp.h"
#include "boost/make_shared.hpp"
#include <iostream>

std::shared_ptr<GUI::GUIApp> gui = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		CefQuitMessageLoop();
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (gui) {
			// Resize the browser window and address bar to match
			//  the new frame
			// window size
			RECT rect;
			GetClientRect(hwnd, &rect);
			HDWP hdwp = BeginDeferWindowPos(1);
			//this->cefAppGUI->
			hdwp = DeferWindowPos(hdwp, gui->getCefCore()->GetBrowserHwnd(),
				NULL, rect.left, rect.top,
				rect.right - rect.left,
				rect.bottom - rect.top,
				SWP_NOZORDER);
			EndDeferWindowPos(hdwp);
		}

		break;

	case WM_ERASEBKGND:
		if (gui) {
			// Dont erase the background if the browser window has
			//  been loaded
			// (this avoids flashing)
			return 0;
		}

		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND RegisterWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = LPCSTR("TestsDatabaseWindow");
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(0,                                 // Optional
																	//  window
																	//  styles.
		LPCSTR("TestsDatabaseWindow"), // Window class
		LPCSTR("CefRemCon"), // Window text
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // Window
												//  style
		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, // Parent window
		NULL, // Menu
		hInstance, // Instance handle
		NULL // Additional application data
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	return hwnd;
}

std::string GetApplicationDir()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR wpath[MAX_PATH];

	GetModuleFileNameW(hModule, wpath, MAX_PATH);
	std::wstring wide(wpath);

	std::string path = CefString(wide);
	path = path.substr(0, path.find_last_of("\\/"));
	return path;
}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND CreateMessageWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc = {
		0
	};

	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MessageWndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = LPCSTR("ClientMessageWindow");
	RegisterClassEx(&wc);
	return CreateWindow(LPCSTR("ClientMessageWindow"), 0, 0, 0, 0, 0, 0,
		HWND_MESSAGE, 0, hInstance, 0);
}

int initCef(HINSTANCE hInstance, int nCmdShow)
{
	CefMainArgs main_args(hInstance);

	CefRefPtr<CEF::CEFApp> app(new CEF::CEFApp);
	//CefRefPtr<CEFApp> app1(app);

	// Execute the secondary process, if any.
	int exit_code = CefExecuteProcess(main_args, app, NULL);

	if (exit_code >= 0) {
		exit(exit_code);
	}

	// Register the window class.
	HWND hwnd = RegisterWindow(hInstance, nCmdShow);
	if (hwnd == 0) {
		return false;
	}

	RECT rect;
	GetClientRect(hwnd, &rect);

	CefSettings settings;
	settings.no_sandbox = true;
	CefString(&settings.log_file).FromASCII("cef_debug.log");
	CefInitialize(main_args, settings, app, nullptr);
	CefWindowInfo info;
	CefBrowserSettings b_settings;
	//CEF::CEFCore* cefCore = new CEF::CEFCore;
	CEF::CEFCore* cefCore = new CEF::CEFCore;
	CefRefPtr<CefClient> client(cefCore);
	//this->setCefCore((CEF::CEFCore*)client.get());
	//gui->setCefCore(new CEF::CEFCore());
	//this->cefClientGUI = (CefRefPtr<CefClient>)this->cefCoreGUI;

	std::string path = "file://" + GetApplicationDir() + "/html/index.html";
	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();

	if (command_line->HasSwitch("url")) {
		path = command_line->GetSwitchValue("url");
	}

	info.SetAsChild(hwnd, rect);
	CefBrowserHost::CreateBrowser(info,
		client, path, b_settings, NULL,
		NULL);
	int result = 0;

	if (!settings.multi_threaded_message_loop) {
		// Run the CEF message loop. This function will block until the
		//  application
		// recieves a WM_QUIT message.
		CefRunMessageLoop();
	}
	else {
		// Create a hidden window for message processing.
		HWND hMessageWnd = CreateMessageWindow(hInstance);
		MSG msg;

		// Run the application message loop.
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DestroyWindow(hMessageWnd);
		hMessageWnd = NULL;
		result = static_cast<int>(msg.wParam);
	}

	CefShutdown();
	return result;
}

int loop() {
	while (gui->getClient()->getClientCore() == nullptr) {
		continue;
	}

	boost::asio::ip::tcp::endpoint endPoint = gui->getClient()->getClientCore()->getEndpoint();

	try {
		gui->getClient()->getClientCore()->connect(endPoint);
		gui->getClient()->getClientCore()->loop();
	}
	catch (boost::system::system_error& err) {
		std::cout << "client terminated " << std::endl;
		return 0;
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	//int result = Client::GUIApp::create(hInstance, nCmdShow);

	std::shared_ptr<Client::ClientApp> client = std::make_shared<Client::ClientApp>();
	CefRefPtr<CEF::CEFApp> cef(new CEF::CEFApp);
	gui = std::make_shared<GUI::GUIApp>(client, cef, hInstance, nCmdShow);
	initCef(hInstance, nCmdShow);

	/*boost::thread_group threads;
	try {
		threads.create_thread(boost::bind(loop));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
	catch (boost::system::system_error& err) {
		std::cout << "client terminated " << std::endl;
		return 0;
	}*/

	return 1;
}