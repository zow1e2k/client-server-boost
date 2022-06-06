#pragma once

#include "CEFApp.h"

namespace CEF {
	enum class JsArgsT {
		BOOL,
		INT,
		DOUBLE,
		STRING,
		ARRAY,
		FUNCTION,
		OBJECT,
		ANY
	};

	bool FuncDef2CefListValue(const CefString& func_name, const std::vector<JsArgsT> args_def,
		const CefV8ValueList& source, CefRefPtr<CefListValue> target);
	bool CefListValue2FuncArgs(const std::vector<JsArgsT> args_def,
		CefRefPtr<CefListValue> source, CefV8ValueList& target);

	void CefV8Array2ListValue(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
	void CefListValue2V8Array(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);
	void CefV8JsonObject2DictionaryValue(CefRefPtr<CefV8Value> source, CefRefPtr<CefDictionaryValue> target);
	void CefDictionaryValue2V8JsonObject(CefRefPtr<CefDictionaryValue> source, CefRefPtr<CefV8Value> target);
}