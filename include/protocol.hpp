#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <memory>
#include "manager.hpp"
#include "client.hpp"

namespace TENTA {
    const std::string REG = "REG";
    const std::string BND = "BND";
    const std::string MSG = "MSG";
    const std::string EXT = "EXT";

    void reg(const std::string& name, std::shared_ptr<Client>& client, ClientManager& cm);
    void bnd(const std::string& target_name, std::shared_ptr<Client>& client, ClientManager& cm);
    void msg(const std::string& text, std::shared_ptr<Client>& client, ClientManager& cm);
    void ext(std::shared_ptr<Client>& client);
}

#endif //PROTOCOL_H