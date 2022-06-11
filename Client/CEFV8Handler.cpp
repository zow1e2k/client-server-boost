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

				Client::start(username, ip);
				std::string args = (std::string)username + "|" + (std::string)password;
				std::string result = Client::exec("[login]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "GetLSInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString dir = arguments[0]->GetStringValue();
				std::string args = (std::string)dir;
				std::string result = Client::exec("[get_ls]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "ReadFileInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString filename = arguments[0]->GetStringValue();
				std::string args = (std::string)filename;
				std::string result = Client::exec("[cat_file]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		}

		return false;
	}
}