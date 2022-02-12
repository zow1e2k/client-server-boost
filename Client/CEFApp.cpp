#include "CEFApp.h"

#include "CEFCore.h"
#include "CEFV8Handler.h"
#include "include/wrapper/cef_helpers.h"

namespace CEF {

	CEFApp::CEFApp() {
	}

	void CEFApp::OnWebKitInitialized() {
		std::string appJS =
			"var app;"
			"if (!app)"
			"    app = {};"
			"(function() {"
			"	app.LoginInJS = function(text) {"
			"		native function LoginInJS();"
			"		return LoginInJS(text);"
			"	};"
			"    app.CheckConnectInJS = function(text) {"
			"        native function CheckConnectInJS();"
			"        return CheckConnectInJS(text);"
			"	};"
			"})();";

		/*std::string appJS = "";
		appJS += "var app;";
		appJS += "if (!app)";
		appJS += "	app = {};";
		appJS += "(function()) {";
		appJS += "	app.CheckConnectInJS = function(text) {";
		appJS += "		native function CheckConnectInJS();";
		appJS += "		return CheckConnectInJS(text);";
		appJS += "	};";
		appJS += "})();";*/

		CefRegisterExtension("v8/app", appJS, new CEFV8Handler(this));
	}
}