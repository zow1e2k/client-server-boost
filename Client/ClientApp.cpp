#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include "ClientCore.h"
#include "ClientApp.h"
#include <iostream>

namespace Client {

	using namespace boost::asio;

	std::shared_ptr<ClientCore> clientPtr;

	int run_client(const std::string& client_name, const std::string& ip) {
		ip::tcp::endpoint ep(ip::address::from_string(ip), 8001);
		
		io_service service;
		//ClientCore::ClientCore client = ClientCore::ClientCore(client_name, service);
		ClientCore client(client_name, service);
		//clientPtr = client.getptr(). //std::make_shared<ClientCore>(client);
		clientPtr = std::make_shared<ClientCore>(client_name, service);

		try {
			clientPtr.get()->connect(ep);
			clientPtr.get()->loop();
			//client.connect(ep);
			//client.loop();
		}
		catch (boost::system::system_error& err) {
			std::cout << "client terminated " << clientPtr.get()->username()
				<< ": " << err.what() << std::endl;
			return 0;
		}

		return 1;
	}

	std::string exec(const std::string& packetName) {
		std::string result = clientPtr.get()->exec(packetName);
		return result;
	}

	int start(const std::string& userName, const std::string& ip) {
		boost::thread_group threads;

		try {
			threads.create_thread(boost::bind(Client::run_client, userName, ip));
			boost::this_thread::sleep(boost::posix_time::millisec(100));
		}
		catch (boost::system::system_error& err) {
			std::cout << "client terminated " << clientPtr.get()->username()
				<< ": " << err.what() << std::endl;
			return 0;
		}

		return 1;
		
		/*threads.create_thread(boost::bind(CefRunMessageLoop));
		boost::this_thread::sleep(boost::posix_time::millisec(100));*/

		//threads.join_all();
	}
}