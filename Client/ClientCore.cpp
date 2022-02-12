#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "ClientCore.h"

namespace Client {

	using namespace boost::asio;
	using namespace boost::placeholders;
	using socket_t = ip::tcp::socket;

	ClientCore::ClientCore(const std::string& username, io_service &service) : socket_(service), started_(true), username_(username) {
	}

	socket_t& ClientCore::socket() {
		return socket_; 
	}

	std::string ClientCore::username() const {
		return username_;
	}

	void ClientCore::connect(ip::tcp::endpoint endPoint) {
		socket_.connect(endPoint);
		return;
	}

	void ClientCore::loop() {
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

	void ClientCore::writeCommand() {
		std::string msg = "";
		std::cout << "Insert the command\n";
		std::cin >> msg;
		//msg.append("\n");
		sendPacket(msg + "\n");

		return;
	}

	void ClientCore::receivePacket() {
		already_read_ = 0;
		read(
			socket_, buffer(buff_),
			boost::bind(&ClientCore::read_complete, this, _1, _2)
		);
		parsePacket();

		return;
	}

	void ClientCore::parsePacket() {
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
		else if (msg.find("gamemodeDestroyed") == 0) {
			OnGamemodeDestroyed(msg);
		}
		else if (msg.find("dirInfoShowed") == 0) {
			OnGamemodeDirShowed(msg);
		}
		else if (msg.find("gamemodeUpload") == 0) {
			OnGamemodeUpload();
		}

		return;
	}

	void ClientCore::OnGamemodeUpload() {
		system("pscp Z:/Projects/Evolve/evolve-rp.ru/gamemodes/evolve_last_last.amx root@188.120.229.168:/root/evolve/evolve-rp.ru/gamemodes");

		std::cout << "[Server] Gamemode uploaded" << std::endl;

		return;
	}

	void ClientCore::OnGamemodeDirShowed(const std::string& msg) {
		std::cout << "[Server] Dir info:" << std::endl;
		std::cout << msg << std::endl;
		return;
	}

	void ClientCore::OnGamemodeDestroyed(const std::string& msg) {
		std::cout << "[Server] Gamemode destroyed:" << std::endl;
		std::cout << msg << std::endl;
		return;
	}

	void ClientCore::OnClientLogin() {
		std::cout << username_ << " logged in" << std::endl;
		AskClients();

		return;
	}

	void ClientCore::OnClientPing(const std::string& msg) {
		std::istringstream in(msg);
		std::string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed") {
			AskClients();
		}

		return;
	}

	void ClientCore::OnInvalidAnswer(const std::string& msg) {
		std::cout << "[Server] Invalid message" << std::endl;
		return;
	}

	void ClientCore::OnClientsList(const std::string& msg) {
		std::string clients = msg.substr(8);
		std::cout << username_ << ", new client list:" << clients;

		return;
	}

	void ClientCore::AskClients() {
		sendPacket("ask_clients\n");
		receivePacket();

		return;
	}

	void ClientCore::sendPacket(const std::string& msg) {
		socket_.write_some(buffer(msg));

		return;
	}

	size_t ClientCore::read_complete(const boost::system::error_code& err, size_t bytes) {
		if (err) {
			return 0;
		}

		already_read_ = bytes;
		bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
		// we read one-by-one until we get to enter, no buffering
		return found ? 0 : 1;
	}
}