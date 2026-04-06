#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <memory>
#include "models/client_manager.hpp"
#include "models/client.hpp"
#include "db/database.hpp"

namespace TENTA {
    const std::string REG = "REG";
    const std::string AUT = "AUT";
    const std::string BND = "BND";
    const std::string MSG = "MSG";
    const std::string LST = "LST";
    const std::string EXT = "EXT";

    void reg(const std::string& name, std::shared_ptr<Client>& client, ClientManager& cm, Database& db);
    void aut(const std::string& name, const std::string& password, std::shared_ptr<Client>& client, Database& db);
    void bnd(const std::string& target_name, std::shared_ptr<Client>& client, ClientManager& cm);
    void msg(const std::string& text, std::shared_ptr<Client>& client, ClientManager& cm);
    void lst(std::shared_ptr<Client>& client, ClientManager& cm, Database& db);
    void ext(std::shared_ptr<Client>& client);
}

#endif //PROTOCOL_H