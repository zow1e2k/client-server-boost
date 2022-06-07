#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include "ClientApp.h"
#include <iostream>

namespace Client {

	using namespace boost::asio;

	ClientApp::ClientApp()
	{
	}

	ClientApp::~ClientApp()
	{
	}

	int ClientApp::run_client(const std::string& client_name, const std::string& ip) {
		ip::tcp::endpoint ep(ip::address::from_string(ip), 8001);
		
		io_service service;
		this->createClientCore(client_name, service);
		this->getClientCore()->setEndpoint(ep);

		return 1;
	}

	void ClientApp::execLS() {
		this->getClientCore().get()->execLS();
		return;
	}

	/*int ClientApp::start(const std::string& userName, const std::string& ip) {
		boost::thread_group threads;

		try {
			threads.create_thread(boost::bind(&run_client, userName, ip));
			boost::this_thread::sleep(boost::posix_time::millisec(100));
		}
		catch (boost::system::system_error& err) {
			std::cout << "client terminated " << this->getClientCore().get()->username()
				<< ": " << err.what() << std::endl;
			return 0;
		}

		return 1;
		
		threads.create_thread(boost::bind(CefRunMessageLoop));
		boost::this_thread::sleep(boost::posix_time::millisec(100));

		//threads.join_all();
	}*/

	void ClientApp::createClientCore(const std::string& username, io_service& service)
	{
		this->clientCore = std::make_shared<ClientCore>(username, service);
	}

	void ClientApp::setClientCore(std::shared_ptr<ClientCore> core)
	{
		this->clientCore = core;
	}

	std::shared_ptr<ClientCore> ClientApp::getClientCore()
	{
		return this->clientCore;
	}
}