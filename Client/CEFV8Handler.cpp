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
		} else if (name == "GetProccessesInJS") {
			std::string args = "";
			std::string result = Client::exec("[top]", args);
			retval = CefV8Value::CreateString(result);
			return true;
		} else if (name == "GetClientsInJS") {
			std::string args = "";
			std::string result = Client::exec("[get_clients]", args);
			retval = CefV8Value::CreateString(result);
			return true;
		} else if (name == "SendMsgInJS") {
			if ((arguments.size() == 2) && arguments[0]->IsString() && arguments[1]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				CefString message = arguments[1]->GetStringValue();
				std::string args = (std::string)username + "|" + (std::string)message;
				std::string result = Client::exec("[send_msg]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "GetMessagesInJS") {
			std::string result = Client::getMessages();
			retval = CefV8Value::CreateString(result);
			return true;
		} else if (name == "CheckLogInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				std::string args = (std::string)username;
				std::string result = Client::exec("[check_log]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "ChangeRoleInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				std::string args = (std::string)username;
				std::string result = Client::exec("[change_role]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "CreateUserInJS") {
			if ((arguments.size() == 3) && arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				CefString password = arguments[1]->GetStringValue();
				CefString role = arguments[2]->GetStringValue();

				std::string args = (std::string)username + "|" + (std::string)password + "|" + (std::string)role;
				std::string result = Client::exec("[create_user]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "DisconnectUserInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString username = arguments[0]->GetStringValue();
				std::string args = (std::string)username;
				std::string result = Client::exec("[disconnect_user]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "ExecFileInJS") {
			if ((arguments.size() == 2) && arguments[0]->IsString() && arguments[1]->IsString()) {
				CefString dir = arguments[0]->GetStringValue();
				CefString filename = arguments[1]->GetStringValue();
				std::string args = (std::string)dir + "|" + (std::string)filename;
				std::string result = Client::exec("[exec_file]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "DownloadFileInJS") {
			if ((arguments.size() == 2) && arguments[0]->IsString() && arguments[1]->IsString()) {
				CefString dir = arguments[0]->GetStringValue();
				CefString filename = arguments[1]->GetStringValue();
				std::string args = (std::string)dir + "|" + (std::string)filename;
				std::string result = Client::exec("[download_file]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		} else if (name == "SendCMDInJS") {
			if ((arguments.size() == 1) && arguments[0]->IsString()) {
				CefString cmdname = arguments[0]->GetStringValue();
				std::string args = (std::string)cmdname;
				std::string result = Client::exec("[send_cmd]", args);
				retval = CefV8Value::CreateString(result);
				return true;
			}
		}
		else if (name == "LogoutInJS") {
			std::string args = "";
			std::string result = Client::exec("[logout]", args);
			Client::destroy();
			retval = CefV8Value::CreateString(result);
			return true;
		}

		return false;
	}
}