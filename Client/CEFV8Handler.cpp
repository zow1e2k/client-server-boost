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
					if (Client::run_client(username, ip) == 0) {
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
						jscall += " successfully connected";
						jscall += "');";
					}
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
		} else if (name == "EvolveUploadInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString file = arguments[0]->GetStringValue();
				CefRefPtr<CefFrame> frame =
					CefV8Context::GetCurrentContext()->GetBrowser()
					->
					GetMainFrame();

				//Client::client.get()->uploadEvolveGamemode(file);

				std::string jscall = "EvolveUpload('";
				jscall += file;
				jscall += " uploaded successfully');";
				frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);

				return true;
			}
		}

		return false;
	}
}