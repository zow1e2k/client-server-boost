#include "WebKernelConvertor.h"
//#include "CEFApp.h"

#include "CEFCore.h"
#include "CEFV8Handler.h"
#include "include/wrapper/cef_helpers.h"

namespace CEF {

	CefRefPtr<CEFV8Handler> cefV8Handler = nullptr;

	CEFApp::CEFApp()
	{
	}

	bool CEFApp::OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message
	) {
		std::string msg_name = message->GetName().ToString();

		if (msg_name == js_execute) {
			CefRefPtr<CefListValue> msg = message->GetArgumentList();
			std::size_t i = 0;
			std::string func_name = msg->GetString(i++).ToString();

			auto it = js_funcs.find(func_name);

			if (it == js_funcs.end() || !cefV8Handler->getJSCallbacks().count(func_name)) {
				return false;
			}

			JsCallback js_callback = cefV8Handler->getJSCallbacks()[func_name];

			CefV8ValueList args;
			auto args_def = it->second;

			if (!CefListValue2FuncArgs(args_def, msg, args)) {
				return false;
			}

			js_callback.func->ExecuteFunctionWithContext(js_callback.ctx, nullptr, args);
			return true;
		}
		return false;
	}

	void CEFApp::OnContextCreated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context
	) {
		CefRefPtr<CefV8Value> window_object = context->GetGlobal();
		cefV8Handler = new CEFV8Handler(this);

		for (auto func : native_funcs) {
			window_object->SetValue(
				func.first,
				CefV8Value::CreateFunction(func.first, cefV8Handler),
				V8_PROPERTY_ATTRIBUTE_READONLY
			);
		}
	}

	/*void CEFApp::OnWebKitInitialized() {
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
			"	app.ShowLSInJS = function() {"
			"        native function ShowLSInJS();"
			"        return ShowLSInJS();"
			"	};"
			"})();";

		//CEFV8Handler cefV8Handler(CEFV8Handler(this));
		//CEF::cefHandler = new CEFV8Handler(this);
		//CEF::cefHandler = 
		//g_cefHandler = (CEFV8Handler*) new CEFV8Handler(this);
		//CefRefPtr<CefV8Handler> cefPtr(g_cefHandler);
		//CefRefPtr<CefV8Handler> handl(new CEFV8Handler(this));
		//CefRegisterExtension("v8/app", appJS, new CEFV8Handler(this));
		//this->setCefCore((CEF::CEFCore*)client.get());
		//guiApp = (GUIApp*)gua.get();
		//CEFV8Handler* ch = new CEFV8Handler(this);
		//CefRefPtr<CefV8Handler>* handl = (CefRefPtr<CefV8Handler>*)ch;
		//CEF::gcfh = handl;
		//CefV8Handler* handl(new CEFV8Handler(this));
		cefV8Handler = new CEFV8Handler(this);
		CefRegisterExtension("v8/app", appJS, cefV8Handler);
	}*/
}
