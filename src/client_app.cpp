#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;
using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io;
        tcp::socket socket(io);
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "8080");
        boost::asio::connect(socket, endpoints);

        cout << "Connected to OctoLink server!\n";
        cout << "Type messages, press Enter to send. Type '/exit' to quit.\n";

        atomic<bool> running = true;

        thread receiver([&socket, &running]() {
                char data[1024];
                boost::system::error_code ec;
                while (running) {
                    size_t len = socket.read_some(boost::asio::buffer(data), ec);
                    if (ec == boost::asio::error::eof) break;
                    if (ec) {
                        if (running) {
                            cerr << "Connection lost: " << ec.message() << endl;
                        }
                        break;
                    }
                    cout << "\r[Server]: " << string(data, len) << "\n> " << flush;
                }
            });

        string line;
        while (getline(cin, line)) {
            if (line == "/exit") {
                running = false;
                socket.close();
                break;
            }
            try {
                boost::asio::write(socket, boost::asio::buffer(line + "\n"));
                cout << "> " << flush;
            } catch (exception& e) {
                cout << "send error: " << e.what() << endl;
                break;
            }

        }

        this_thread::sleep_for(chrono::milliseconds(100));

        if (receiver.joinable()) {receiver.join();}


    } catch (exception& e) {
        cout << "Caught exception: " << e.what() << std::endl;
    }
}