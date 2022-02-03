#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "Core.h"

namespace Client {

	using namespace boost::asio;

	ip::tcp::endpoint ep(ip::address::from_string("188.120.229.168"), 8001);
	void run_client(const std::string& client_name) {
		io_service service;
		Core client(client_name, service);

		try {
			client.connect(ep);
			client.loop();
		}
		catch (boost::system::system_error& err) {
			std::cout << "client terminated " << client.username()
				<< ": " << err.what() << std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	boost::thread_group threads;

	std::cout << "You're welcome. Enter your name below\n";
	std::string playerName;
	std::cin >> playerName;

	threads.create_thread(boost::bind(Client::run_client, playerName));
	boost::this_thread::sleep(boost::posix_time::millisec(100));

	threads.join_all();

	return 1;
}