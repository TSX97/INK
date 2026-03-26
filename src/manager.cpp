#include "manager.hpp"
#include <vector>
#include <memory>
#include <mutex>

using namespace std;
ClientManager::ClientManager() = default;
ClientManager::~ClientManager() = default;

void ClientManager::add(std::shared_ptr<Client> client) {
    lock_guard<mutex> lock(mtx);
    clients.push_back(move(client));

}

void ClientManager::remove_by_name(const std::string &name) {
    lock_guard<mutex> lock(mtx);
    clients.erase(
    std::remove_if(clients.begin(), clients.end(),
        [&name](const auto& client) { return client->name == name; }),
    clients.end());
}

void ClientManager::remove_by_id(int id) {
    lock_guard<mutex> lock(mtx);
    clients.erase(
        remove_if(clients.begin(), clients.end(),
    [id](const auto& client){return client->id == id;}), clients.end());

}

shared_ptr<Client> ClientManager::find_by_name(const string& name) {
    lock_guard<mutex> lock(mtx);
    for (const auto& client : clients) {
        if (client->name == name) {
            return client;
        }
    }
    return nullptr;

}

shared_ptr<Client> ClientManager::find_by_id(int id) {
    lock_guard<mutex> lock(mtx);
    for (const auto& client : clients) {
        if (client->id == id) {
            return client;
        }
    }
    return nullptr;
}

vector<std::shared_ptr<Client>> ClientManager::get_all() {
    lock_guard<mutex> lock(mtx);
    return clients;
}
