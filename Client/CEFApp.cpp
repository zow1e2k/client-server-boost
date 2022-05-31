#include "CEFApp.h"

#include "CEFCore.h"
#include "CEFV8Handler.h"
#include "include/wrapper/cef_helpers.h"

namespace CEF {

	CEFApp::CEFApp()
	{
	}

	void CEFApp::OnWebKitInitialized() {
		std::string appJS =
			"var app;"
			"if (!app)"
			"    app = {};"
			"(function() {"
			"	app.LoginInJS = function(username, password, ip) {"
			"		native function LoginInJS();"
			"		return LoginInJS(username, password, ip);"
			"	};"
			"    app.CheckConnectInJS = function(text) {"
			"        native function CheckConnectInJS();"
			"        return CheckConnectInJS(text);"
			"	};"
			"    app.EvolveUploadInJS = function(text) {"
			"        native function EvolveUploadInJS();"
			"        return EvolveUploadInJS(text);"
			"	};"
			"    app.GetLSInJS = function() {"
			"        native function GetLSInJS();"
			"        return GetLSInJS();"
			"	};"
			"})();";

		//CEFV8Handler cefV8Handler(CEFV8Handler(this));
		//CEF::cefHandler = new CEFV8Handler(this);
		//CEF::cefHandler = 
		g_cefHandler = (CEFV8Handler*) new CEFV8Handler(this);
		CefRefPtr<CefV8Handler> cefPtr(g_cefHandler);
		//CefRefPtr<CefV8Handler> CEF::cefHandler(new CEFV8Handler(this));
		//CefRegisterExtension("v8/app", appJS, new CEFV8Handler(this));
		//this->setCefCore((CEF::CEFCore*)client.get());
		//guiApp = (GUIApp*)gua.get();
		CefRegisterExtension("v8/app", appJS, cefPtr);
	}
}