#pragma once
#include "include/cef_app.h"
#include "WebKernelConvertor.h"
#include <unordered_map>

#define AddFunc(name) const std::string name = #name;

namespace CEF {

	const std::string js_execute = "app";
	struct {
		AddFunc(Login);
		AddFunc(AddV8Callback);
	} native_func_names;

	struct {
		AddFunc(ShowLS);
	} js_func_names;

	const std::unordered_map<std::string, const std::vector<JsArgsT>> native_funcs = {
		{native_func_names.Login, { JsArgsT::STRING, JsArgsT::STRING, JsArgsT::STRING }},
		{native_func_names.AddV8Callback, { JsArgsT::STRING, JsArgsT::FUNCTION }}
	};

	const std::unordered_map<std::string, const std::vector<JsArgsT>> js_funcs = {
		{js_func_names.ShowLS, { JsArgsT::STRING }}
	};

	struct JsCallback {
		JsCallback() = default;
		JsCallback(CefRefPtr<CefV8Value> func, CefRefPtr<CefV8Context> ctx)
			: func(func), ctx(ctx) {};
		CefRefPtr<CefV8Value> func;
		CefRefPtr<CefV8Context> ctx;
	};

	struct CEFV8Handler : public CefV8Handler {
		CEFV8Handler(CefRefPtr<CefApp> app);

		bool Execute(const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) OVERRIDE;

		std::unordered_map<std::string, JsCallback> getJSCallbacks();

	private:
		CefRefPtr<CefApp> app;
		std::unordered_map<std::string, JsCallback> js_callbacks;

		IMPLEMENT_REFCOUNTING(CEFV8Handler);
	};
}