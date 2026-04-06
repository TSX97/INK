#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include "models/client_manager.hpp"
#include <memory>
#include "db/database.hpp"

class Server {
private:
    boost::asio::io_context io_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ClientManager client_manager_;
    Database db_;

    void handle_client(std::shared_ptr<Client> client);
    void accept();

public:
    Server(short port, Database& db);
    ~Server();
    void run();
    void stop();
};


#endif