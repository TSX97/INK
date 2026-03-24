#include <string>
#include <boost/asio.hpp>
#include "client.hpp"

using namespace std;
using boost::asio::ip::tcp;



Client::Client(tcp::socket s, int i): name("User" + to_string(i)), id(i), sock(move(s)){}

void Client::send(const string& message) {
        boost::asio::write(sock, boost::asio::buffer(message + "\n"));
    }
