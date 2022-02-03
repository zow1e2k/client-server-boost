#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Core.h"

namespace Client {

	using namespace boost::asio;
	using namespace boost::placeholders;
	using socket_t = ip::tcp::socket;

	Core::Core(const std::string& username, io_service &service) : socket_(service), started_(true), username_(username) {}

	socket_t& Core::socket() {
		return socket_; 
	}

	std::string Core::username() const {
		return username_;
	}

	void Core::connect(ip::tcp::endpoint endPoint) {
		socket_.connect(endPoint);
		return;
	}

	void Core::loop() {
		sendPacket("login " + username_ + "\n");
		receivePacket();

		while (started_) {
			writeCommand();
			receivePacket();

			int millis = 2;
			std::cout << username_ << " postpone ping " << millis << " ms" << std::endl;
			boost::this_thread::sleep(boost::posix_time::millisec(millis));
		}

		return;
	}

	void Core::writeCommand() {
		std::string msg = "";
		std::cout << "Insert the command\n";
		std::cin >> msg;
		//msg.append("\n");
		sendPacket(msg + "\n");

		return;
	}

	void Core::receivePacket() {
		already_read_ = 0;
		read(
			socket_, buffer(buff_),
			boost::bind(&Core::read_complete, this, _1, _2)
		);
		parsePacket();

		return;
	}

	void Core::parsePacket() {
		std::string msg(buff_, already_read_);

		if (msg.find("login ") == 0) {
			OnClientLogin();
		}
		else if (msg.find("ping ") == 0) {
			OnClientPing(msg);
		}
		else if (msg.find("clients ") == 0) {
			OnClientsList(msg);
		}
		else if (msg.find("[Server] Invalid message") == 0) {
			OnInvalidAnswer(msg);
		}

		return;
	}

	void Core::OnClientLogin() {
		std::cout << username_ << " logged in" << std::endl;
		AskClients();

		return;
	}

	void Core::OnClientPing(const std::string& msg) {
		std::istringstream in(msg);
		std::string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed") {
			AskClients();
		}

		return;
	}

	void Core::OnInvalidAnswer(const std::string& msg) {
		std::cout << "[Server] Invalid message" << std::endl;
		return;
	}

	void Core::OnClientsList(const std::string& msg) {
		std::string clients = msg.substr(8);
		std::cout << username_ << ", new client list:" << clients;

		return;
	}

	void Core::AskClients() {
		sendPacket("ask_clients\n");
		receivePacket();

		return;
	}

	void Core::sendPacket(const std::string& msg) {
		socket_.write_some(buffer(msg));

		return;
	}

	size_t Core::read_complete(const boost::system::error_code& err, size_t bytes) {
		if (err) {
			return 0;
		}

		already_read_ = bytes;
		bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
		// we read one-by-one until we get to enter, no buffering
		return found ? 0 : 1;
	}
}