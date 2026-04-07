#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <nlohmann/json.hpp>
#include "tools.hpp"

using namespace std;
using boost::asio::ip::tcp;
using json = nlohmann::json;

int main() {
    try {
        boost::asio::io_context io;
        tcp::socket socket(io);
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "8080");
        boost::asio::connect(socket, endpoints);
        ;
        cout << "Connected to Ink server!" << endl;

        string response;
        cout << "Do you have an account? [Y/n]" << endl;
        cin >> response;
        cout << "Enter login: ";
        string login;
        cin >> login;
        cout << "Enter password: ";
        string password;
        cin >> password;

        if (response == "Y" || response == "y") {
                json aut = {{"cmd", "AUT"}, {"login", login}, {"password", password}};
                boost::asio::write(socket, boost::asio::buffer(aut.dump() + "\n"));
        } else if (response == "N" || response == "n") {
            json reg = {{"cmd", "REG"}, {"login", login}, {"password", password}};
            boost::asio::write(socket, boost::asio::buffer(reg.dump() + "\n"));
        } else {
            cout << "Invalid response." << endl;
        }


        char resp_buffer[1024];
        boost::system::error_code ec2;
        size_t resp_len = socket.read_some(boost::asio::buffer(resp_buffer), ec2);
        if (!ec2) {
            string resp(resp_buffer, resp_len);
            cout << "Server: " << resp << endl;
        }

        cout << "Type messages. Use 'BND <name>' to select recipient, then 'MSG <text>'.\n";

        atomic<bool> running = true;


        thread receiver([&socket, &running]() {
            char data[1024];
            boost::system::error_code ec;
            while (running) {
                size_t len = socket.read_some(boost::asio::buffer(data), ec);
                if (ec == boost::asio::error::eof) break;
                if (ec) break;
                cout << "\r[Server]: " << string(data, len) << "\n> " << flush;
            }
        });


        string line;
        while (getline(cin, line)) {
            if (line == "EXT") {
                running = false;
                socket.close();
                break;
            }

            json j = tools::user_command_to_json(line);
            boost::asio::write(socket, boost::asio::buffer(j.dump() + "\n"));
        }


        this_thread::sleep_for(chrono::milliseconds(100));
        if (receiver.joinable()) {
            receiver.join();
        }

    } catch (exception& e) {
        cout << "Caught exception: " << e.what() << endl;
    }
    return 0;
}