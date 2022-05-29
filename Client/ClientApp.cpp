#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "ClientCore.h"
#include "CEFApp.h"
#include "CEFCore.h"
#include <Windows.h>
#include "ClientApp.h"

namespace Client {

	using namespace boost::asio;

	//std::shared_ptr<ClientCore> clientPtr;

	int run_client(const std::string& client_name, const std::string& ip) {
		ip::tcp::endpoint ep(ip::address::from_string(ip), 8001);

		io_service service;
		//ClientCore::ClientCore client = ClientCore::ClientCore(client_name, service);
		ClientCore client(client_name, service);
		//clientPtr = client.getptr(). //std::make_shared<ClientCore>(client);
		//clientPtr = std::make_shared<ClientCore>(client_name, service);

		try {
			//clientPtr.get()->connect(ep);
			client.connect(ep);
			//client.loop();
		}
		catch (boost::system::system_error& err) {
			/*std::cout << "client terminated " << clientPtr.get()->username()
				<< ": " << err.what() << std::endl;*/
			return 0;
		}

		return 1;
	}

	void start() {
		/*boost::thread_group threads;

		threads.create_thread(boost::bind(Client::run_client, "zow1k"));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
		threads.create_thread(boost::bind(CefRunMessageLoop));
		boost::this_thread::sleep(boost::posix_time::millisec(100));

		threads.join_all();*/

		return;
	}
}

int main(int argc, char* argv[]) {
	/*boost::thread_group threads;

	std::cout << "You're welcome. Enter your name below\n";
	std::string playerName;
	std::cin >> playerName;

	threads.create_thread(boost::bind(Client::run_client, playerName));
	boost::this_thread::sleep(boost::posix_time::millisec(100));

	threads.join_all();*/

	return 1;
}