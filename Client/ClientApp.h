#pragma once

namespace Client {

	int run_client(const std::string& client_name, const std::string& ip);
	int start(const std::string& client_name, const std::string& ip);
	std::string exec(const std::string& packetName, const std::string& packetArgs);
	std::string getMessages();
	int destroy();
}
