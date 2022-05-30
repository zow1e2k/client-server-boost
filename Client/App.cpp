#include <string>
#include <algorithm>
#include "GUIApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	int result = Client::GUIApp::create(hInstance, nCmdShow);
	return result;
}