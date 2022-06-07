#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace boost::asio;
using namespace boost::posix_time;
io_service service;

class talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
array clients;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)


void update_clients_changed();

/** simple connection to server:
	- logs in just with username (no password)
	- all connections are initiated by the client: client asks, server answers
	- server disconnects any client that hasn't pinged for 5 seconds
	Possible client requests:
	- gets a list of all connected clients
	- ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class talk_to_client : public boost::enable_shared_from_this<talk_to_client>
	, boost::asio::noncopyable {
	typedef talk_to_client self_type;
	talk_to_client() : sock_(service), started_(false),
		timer_(service), clients_changed_(false) {
	}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start() {
		started_ = true;
		clients.push_back(shared_from_this());
		last_ping = boost::posix_time::microsec_clock::local_time();
		// first, we wait for client to login
		do_read();
	}

	static ptr new_() {
		ptr new_(new talk_to_client);
		return new_;
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();

		ptr self = shared_from_this();
		array::iterator it = std::find(clients.begin(), clients.end(), self);
		clients.erase(it);
		update_clients_changed();
	}

	bool started() const { return started_; }
	ip::tcp::socket& sock() { return sock_; }
	std::string username() const { return username_; }
	void set_clients_changed() { clients_changed_ = true; }

private:
	void on_read(const error_code& err, size_t bytes) {
		if (err) stop();
		if (!started()) return;
		// process the msg
		std::string msg(read_buffer_, bytes);
		if (msg.find("login ") == 0) {
			on_login(msg);
		}
		else if (msg.find("ping") == 0) {
			on_ping();
		}
		else if (msg.find("ask_clients") == 0) {
			on_clients();
		}
		else if (msg.find("[get_ls]") == 0) {
			OnGamemodeDirInfo();
		}
		/*else if (msg.find("gamemodeUpload") == 0) {
			OnGamemodeUpload();
		} else if (msg.find("[evolve_destroy]" == 0) {
			EvolveDestroy(msg);
		}  else if (msg.find("[evolve_destroy]" == 0) {
			EvolveDestroy(msg);
		}*/
		else {
			//std::cerr << "[Console] Invalid message: " << msg << std::endl;
			on_invalid();
		}
	}

	/*void OnGamemodeUpload() {
		do_write("gamemodeUpload\n");

		return;
	}*/

	/*void EvolveDestroy(std::string& msg) {
		std::size_t posLit = msg.find("]");
		std::string file = msg.substr(posLit);

		std::string rmString = "rm /root/evolve/evolve-rp.ru/gamemodes/evolve_last_last.amx >> /root/Server/" + username() + "/tmpLog.txt 2>&1";
		system(rmString.c_str());

		std::fstream file;
		std::string output;
		std::string pwdLogFile = "/root/Server/" + username() + "/tmpLog.txt";
		file.open(pwdLogFile);

		if (file) {
			file >> output;
		}

		std::string removeLogString = "rm -r /root/Server/" + username();
		system(removeLogString.c_str());

		do_write("[evolve_destroyed]" + output + "\n");


		return;
	}*/

	void OnGamemodeDirInfo() {
		std::string makeDirString = "mkdir /root/Server/" + username();
		std::string writeLogString = "ls /root >> /root/Server/" + username() + "/tmpLog.txt 2>&1";

		system(makeDirString.c_str());
		system(writeLogString.c_str());

		std::ifstream file;
		std::string output;
		std::string pwdLogFile = "/root/Server/" + username() + "/tmpLog.txt";
		file.open(pwdLogFile);

		for (std::string line; getline(file, line); ) {
			output += line + ",";
		}

		std::string removeLogString = "rm -r /root/Server/" + username();
		
		system(removeLogString.c_str());

		//do_write("[dirInfoShowed] " + output + "\n");
		//std::cout << "[get_ls][" << username_ << "]" << output << "\n";
		do_write("[get_ls][" + username_ + "]" + output + "\n");
	}

	/*void OnGamemodeDestroy() {
		std::string makeDirString = "mkdir /root/Server/" + username();
		std::string writeLogString = "rm /root/evolve/evolve-rp.ru/gamemodes/evolve_last_last.amx >> /root/Server/" + username() + "/tmpLog.txt 2>&1";

		system(makeDirString.c_str());
		system(writeLogString.c_str());

		std::fstream file;
		std::string output;
		std::string pwdLogFile = "/root/Server/" + username() + "/tmpLog.txt";
		file.open(pwdLogFile);

		if (file) {
			file >> output;
		}

		std::string removeLogString = "rm -r /root/Server/" + username();
		system(removeLogString.c_str());

		do_write("gamemodeDestroyed | " + output + "\n");

		return;
	}*/

	void on_login(const std::string& msg) {
		std::istringstream in(msg);
		in >> username_ >> username_;
		std::cout << username_ << " logged in" << std::endl;
		do_write("login ok\n");
		update_clients_changed();
	}
	void on_ping() {
		do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}

	void on_invalid() {
		do_write("[Server] Invalid message\n");
	}

	void on_clients() {
		std::string msg;
		for (array::const_iterator b = clients.begin(), e = clients.end(); b != e; ++b)
			msg += (*b)->username() + " ";
		do_write("clients " + msg + "\n");
	}

	void do_ping() {
		do_write("ping\n");
	}
	void do_ask_clients() {
		do_write("ask_clients\n");
	}

	void on_check_ping() {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		if ((now - last_ping).total_milliseconds() > 30000) {
			std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
			stop();
		}
		last_ping = boost::posix_time::microsec_clock::local_time();
	}
	void post_check_ping() {
		timer_.expires_from_now(boost::posix_time::millisec(30000));
		timer_.async_wait(MEM_FN(on_check_ping));
	}


	void on_write(const error_code& err, size_t bytes) {
		do_read();
	}
	void do_read() {
		async_read(sock_, buffer(read_buffer_),
			MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		post_check_ping();
	}
	void do_write(const std::string& msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()),
			MEM_FN2(on_write, _1, _2));
	}
	size_t read_complete(const boost::system::error_code& err, size_t bytes) {
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		// we read one-by-one until we get to enter, no buffering
		return found ? 0 : 1;
	}
private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string username_;
	deadline_timer timer_;
	boost::posix_time::ptime last_ping;
	bool clients_changed_;
};

void update_clients_changed() {
	for (array::iterator b = clients.begin(), e = clients.end(); b != e; ++b)
		(*b)->set_clients_changed();
}

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
	client->start();
	talk_to_client::ptr new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}

int main(int argc, char* argv[]) {
	std::cout << "[Console] Server has been started\n";
	talk_to_client::ptr client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
	service.run();
}