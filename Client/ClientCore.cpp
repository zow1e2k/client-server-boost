#define _WIN32_WINNT 0x0601
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include "ClientCore.h"

namespace Client {

	using namespace boost::asio;
	using namespace boost::placeholders;
	using socket_t = ip::tcp::socket;

	ClientCore::ClientCore(const std::string& username, io_service &service) : 
		/*already_read_(500),
		buff_("aaa"),*/
		socket_(service),
		started_(true),
		username_(username)
	{
		//clientGUIApp = GUIApp::getGUIApp();
		//clientGUIApp = Client::guiApp;
		return;
	}

	socket_t& ClientCore::socket() {
		return socket_; 
	}

	std::string ClientCore::username() const {
		return username_;
	}

	void Client::ClientCore::execLS()
	{
		sendPacket("[get_ls]\n");
		return;
	}

	void ClientCore::setUsername(const std::string& username)
	{
	}

	void ClientCore::setEndpoint(boost::asio::ip::tcp::endpoint endPoint)
	{
		this->endPoint = endPoint;
	}

	boost::asio::ip::tcp::endpoint ClientCore::getEndpoint()
	{
		return this->endPoint;
	}

	void ClientCore::connect(ip::tcp::endpoint endPoint) {
		socket_.connect(endPoint);
		return;
	}

	void ClientCore::loop() {
		sendPacket("login " + username_ + "\n");
		receivePacket();

		while (started_) {
			sendPacket((std::string)"empty" + "\n");
			receivePacket();

			int millis = 2;
			//std::cout << username_ << " postpone ping " << millis << " ms" << std::endl;
			boost::this_thread::sleep(boost::posix_time::millisec(millis));
		}

		return;
	}

	void ClientCore::writeCommand() {
		//std::string msg = "";
		//std::cout << "Insert the command\n";
		//std::cin >> msg;
		//msg.append("\n");
		//sendPacket(msg + "\n");

		return;
	}

	std::string ClientCore::receivePacket() {
		already_read_ = 0;
		read(
			socket_, buffer(buff_),
			boost::bind(&ClientCore::read_complete, this, _1, _2)
		);
		
		std::string str = parsePacket();

		return str;
	}

	std::string ClientCore::parsePacket() {
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
		else if (msg.find("gamemodeUpload") == 0) {
			OnGamemodeUpload();
		}
		else if (msg.find("[evolve_destroyed]") == 0) {
			//OnGamemodeUpload();
		}
		else if (msg.find("[dirInfoShowed]") == 0) {
			OnDirInfoShowed(msg);
			return msg;
		}

		return "error";
	}

	void ClientCore::uploadEvolveGamemode(const std::string& file) {
		sendPacket("[evolve_destroy]\n");
		receivePacket();

		std::string command = "pscp " + file + " root@188.120.229.168:/root/evolve/evolve-rp.ru/gamemodes";
		system(command.c_str());

		return;
	}

	void ClientCore::OnGamemodeUpload() {
		system("pscp Z:/Projects/Evolve/evolve-rp.ru/gamemodes/evolve_last_last.amx root@188.120.229.168:/root/evolve/evolve-rp.ru/gamemodes");

		std::cout << "[Server] Gamemode uploaded" << std::endl;

		return;
	}

	void ClientCore::OnDirInfoShowed(const std::string& msg) {
		//guacc->getCefCore()->
		//CefV8ValueList list;
		//CefRefPtr<CefV8Value> val(CefV8Value::CreateString(msg));
		//list.push_back(val);
		//CefString ex("aaa");
		//CefRefPtr<CefV8Value> obj(CefV8Value::);
		//CEF::->Execute("ShowLSInJS", nullptr, list, val, ex);

		//CEFCore* cf = guacc->get()->getCefCore();
		//CefRefPtr<CefBrowser> br = CefV8Context::GetCurrentContext()->GetBrowser();

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