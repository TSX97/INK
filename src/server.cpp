#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include "client.hpp"

using namespace std;
using boost::asio::ip::tcp;

mutex clients_mutex;
vector<shared_ptr<Client>> clients;
int next_id = 1;

void handle_client(shared_ptr<Client> client) {
	while (true) {
		char data[1024] = {0};
		boost::system::error_code err;
		size_t len = client->sock.read_some(boost::asio::buffer(data), err);
		if (err == boost::asio::error::eof) {
			cout << client->name << " closed" << endl;
			break;
 		}
		if (err) {
			cout << "error reading from " << client->name << ": " << err.message() << endl;
			break;
		}
		string msg(data, len);
		while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r')) {
			msg.pop_back();
		}
		cout << client->name << ": " << msg << endl;
		client->send(msg);
	}
	{
		lock_guard<mutex> lock(clients_mutex);
		clients.erase(remove_if(clients.begin(), clients.end(),
			[&client](const shared_ptr<Client>& c) { return c->id == client->id; }),
			clients.end());
	}
}

int main(){
	try {
		boost::asio::io_context io;
		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));
		cout << "acceptor created" << endl;

		while(true) {
			tcp::socket socket(io);
			acceptor.accept(socket);
			cout << "acceptor accepted" << endl;
			int id = next_id++;
			auto client = make_shared<Client>(move(socket), id);
			{
				lock_guard<mutex> lock(clients_mutex);
				clients.push_back(client);

			}
			thread(handle_client, client).detach();

			}
		} catch (exception& e) {
		cout << "Error: " << e.what() << endl;
	}
	return 0;
}
