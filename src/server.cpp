#include "server.hpp"
#include "fishman.hpp"
#include <thread>
#include <boost/asio.hpp>

using namespace std;

Server::Server(short port)
    : acceptor_(io_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    cout << "acceptor created" << endl;
    Fishman::init();
}

Server::~Server() = default;

void Server::run() {
    accept();
    io_.run();
}

void Server::accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_);
    acceptor_.async_accept(*socket,
        [this, socket](boost::system::error_code ec) {
            if (!ec) {
                auto client = std::make_shared<Client>(std::move(*socket), 0);
                client_manager_.add(client);
                std::thread(&Server::handle_client, this, client).detach();
            }
            accept();
        });
}

void Server::handle_client(shared_ptr<Client> client) {
    char buf[1024];
    boost::system::error_code ec;
    while (true) {
        size_t len = client->socket.read_some(boost::asio::buffer(buf), ec);
        if (ec) break;
        std::string raw(buf, len);
        Fishman::handle(raw, client, client_manager_);
    }
}