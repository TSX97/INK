#ifndef FISHMAN_HPP
#define FISHMAN_HPP

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "client.hpp"
#include "manager.hpp"

namespace Fishman {
    void init();
    void handle(const std::string& raw, std::shared_ptr<Client> client, ClientManager& cm);
}

#endif