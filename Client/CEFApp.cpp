#include "CEFApp.h"

#include "CEFCore.h"
#include "CEFV8Handler.h"
#include "include/wrapper/cef_helpers.h"

namespace CEF {

	extern CefV8Handler* cefV8Handler = NULL;

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
			"    app.GetLSInJS = function(dir) {"
			"        native function GetLSInJS();"
			"        return GetLSInJS(dir);"
			"	};"
			"    app.ReadFileInJS = function(filename) {"
			"        native function ReadFileInJS();"
			"        return ReadFileInJS(filename);"
			"	};"
			"})();";

		cefV8Handler = new CEFV8Handler(this);
		CefRegisterExtension("v8/app", appJS, cefV8Handler);
	}
}
