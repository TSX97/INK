#ifndef FISHMAN_HPP
#define FISHMAN_HPP

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "models/client.hpp"
#include "models/client_manager.hpp"
#include "db/database.hpp"

namespace Fishman {
    void init();
    void handle(const std::string& raw, std::shared_ptr<Client> client, ClientManager& cm, Database& db);
}

#endif