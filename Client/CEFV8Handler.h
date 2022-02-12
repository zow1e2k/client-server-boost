#pragma once
#include "include/cef_app.h"

namespace CEF {

	struct CEFV8Handler : public CefV8Handler {
		CEFV8Handler(CefRefPtr<CefApp> app);

		bool Execute(const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) OVERRIDE;

	private:
		CefRefPtr<CefApp> app;

		IMPLEMENT_REFCOUNTING(CEFV8Handler);
	};
}