#include "CEFV8Handler.h"
#include "ClientApp.h"
#include "GUIApp.h"

namespace CEF {

	CEFV8Handler::CEFV8Handler(CefRefPtr<CefApp> app) {
		this->app = app;
	}

	bool CEFV8Handler::Execute(
		const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception
	) {
		if (name == "LoginInJS") {
			if ((arguments.size() == 3) && arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				CefString password = arguments[1]->GetStringValue();
				CefString ip = arguments[2]->GetStringValue();

				CefRefPtr<CefFrame> frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();

				std::string jscall = "";

				if (password.ToString() != "1597538624") {
					jscall += "Login";
					jscall += "('";
					jscall += username;
					jscall += " invalid username and/or password";
					jscall += "');";
				} else {
					if (Client::start(username, ip) == 0) {
						jscall += "Login";
						jscall += "('";
						jscall += username;
						jscall += " cannot connected";
						jscall += "');";
					}
					else {
						jscall += "Login";
						jscall += "('";
						jscall += username;
						jscall += " | " + (std::string)frame->GetURL();
						jscall += " successfully connected";
						jscall += "');";
					}
				}

				frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);
				return true;
			}
		} else if (name == "GetLSInJS") {
			std::string result = Client::exec("[get_ls]");
			retval = CefV8Value::CreateString(result);

			return true;
		}

		return false;
	}
}