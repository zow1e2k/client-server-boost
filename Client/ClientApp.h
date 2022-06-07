#pragma once

#include "ClientCore.h"

namespace Client {

	class ClientApp : public std::enable_shared_from_this<ClientApp> {

	public:
		ClientApp();
		~ClientApp();

		//int start(const std::string& userName, const std::string& ip);
		int run_client(const std::string& client_name, const std::string& ip);
		void execLS();
		void createClientCore(const std::string& username, io_service& service);
		void setClientCore(std::shared_ptr<ClientCore> core);
		std::shared_ptr<ClientCore> getClientCore();

	private:
		std::shared_ptr<ClientCore> clientCore = nullptr;
	};
}
