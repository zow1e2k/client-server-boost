#include "CEFV8Handler.h"
#include "ClientApp.h"

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
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString text = arguments[0]->GetStringValue();

				CefRefPtr<CefFrame> frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();

				std::string jscall = "";

				if (Client::run_client(text) == 0) {
					jscall += "Login";
					jscall += "('";
					jscall += text;
					jscall += " cannot connected";
					jscall += "');";
				} else {
					jscall += "Login";
					jscall += "('";
					jscall += text;
					jscall += " successfully connected";
					jscall += "');";
				}

				frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);

				 /*If you want your method to return a value, just use
				*  retval, like this:
				* retval = CefV8Value::CreateString("Hello World!");
				* you can use any CefV8Value, what means you can return
				*  arrays, objects or whatever you can create with
				*  CefV8Value::Create* methods*/

				return true;
			}
		} else if (name == "CheckConnectInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString text = arguments[0]->GetStringValue();
				CefRefPtr<CefFrame> frame =
					CefV8Context::GetCurrentContext()->GetBrowser()
					->
					GetMainFrame();
				std::string jscall = "CheckConnect('";
				jscall += text;
				jscall += "');";
				frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);

				return true;
			}
		}

		return false;
	}
}