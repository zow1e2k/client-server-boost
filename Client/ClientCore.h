#pragma once

namespace Client {

	using namespace boost::asio;
	using socket_t = boost::asio::ip::tcp::socket;

	class ClientCore : public std::enable_shared_from_this<ClientCore> {

	public:
		ClientCore(const std::string& username, io_service &service);

		//std::shared_ptr<ClientCore> getptr();
		socket_t& socket();
		std::string username() const;
		void connect(ip::tcp::endpoint endPoint);
		void loop();
		void uploadEvolveGamemode(const std::string& file);

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