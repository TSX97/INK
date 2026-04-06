#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>

struct Client {
    std::string name;
    int id;
    int user_id;
    std::string target;
    boost::asio::ip::tcp::socket socket;

    Client(boost::asio::ip::tcp::socket socket, int id);
    void send(const std::string& message);

#ifdef INK_TEST_MODE
    mutable std::vector<std::string> test_log;
    std::vector<std::string> get_log() const { return test_log; }
    void clear_log() { test_log.clear(); }
#endif
};

#endif