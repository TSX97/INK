#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include "client.hpp"

class ClientManager {

private:
    std::vector<std::shared_ptr<Client>> clients;
    std::mutex mtx;

public:
    ClientManager();
    ~ClientManager();

    void add(std::shared_ptr<Client> c);
    void remove_by_name(const std::string& name);
    void remove_by_id(const int& id);
    std::shared_ptr<Client> find_name(const std::string& name);
    std::shared_ptr<Client> find_id(int id);
    std::vector<std::shared_ptr<Client>> get_all();


};


#endif //MANAGER_HPP