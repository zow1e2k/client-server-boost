#pragma once

namespace Client {
	using namespace boost::asio;
	using socket_t = boost::asio::ip::tcp::socket;

	class Core : public std::enable_shared_from_this<Core> {
	public:
		Core(const std::string& username, io_service &service);
		socket_t& socket();
		std::string username() const;
		void connect(ip::tcp::endpoint endPoint);
		void loop();

	private:
		void writeCommand();
		void receivePacket();
		void sendPacket(const std::string& msg);
		void parsePacket();
		void OnClientLogin();
		void OnClientPing(const std::string& msg);
		void OnInvalidAnswer(const std::string& msg);
		void OnClientsList(const std::string& msg);
		void AskClients();
		void OnGamemodeDestroyed(const std::string& msg);
		void OnGamemodeDirShowed(const std::string& msg);
		void OnGamemodeUpload();
		
		size_t read_complete(const boost::system::error_code& err, size_t bytes);

		socket_t socket_;
		enum { max_msg = 1024 };
		size_t already_read_;
		char buff_[max_msg];
		bool started_;
		std::string username_;
	};
}