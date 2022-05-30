#pragma once

namespace Client {

	int run_client(const std::string& client_name, const std::string& ip);
	int start(const std::string& client_name, const std::string& ip);
	void execLS();
}
