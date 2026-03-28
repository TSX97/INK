#include "protocol.hpp"
#include "tools.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace TENTA {
    void reg(const std::string& name, std::shared_ptr<Client>& client, ClientManager& cm) {
        if (name.empty()) {
              client->send(tools::error("name cannot be empty").dump());
            return;
        }

        auto existing =  cm.find_by_name(name);
        if (existing) {
              client->send(tools::error("user already exists").dump());
            return;
        }

        client->name = name;
          client->send(tools::ok("Registered as " + name).dump());
        cout << "User " << client->name << " registered" << std::endl;
    }

    void bnd(const std::string& target_name, std::shared_ptr<Client>& client, ClientManager& cm) {
        if (target_name.empty()) {
              client->send(tools::error("target name cannot be empty").dump());
            return;
        }

        auto target = cm.find_by_name(target_name);
        if (!target) {
              client->send(tools::error("user not found").dump());
            return;
        }

        client->target = target_name;
         client->send(tools::ok("Will send to " + target_name).dump());
        cout << "Client " << client->name << " now targets " << target_name << std::endl;
    }

    void msg(const std::string& text, std::shared_ptr<Client>& client, ClientManager& cm) {

        if (text.empty()) {
              client->send(tools::error("text cannot be empty").dump());
            return;
        }
        auto target_client = cm.find_by_name(client->target);
        if (!target_client) {
              client->send(tools::error("user not found: " + client->target).dump());
            return;
        }

        target_client->send("[PM from " + client->name + "]: " + text);

         client->send(tools::ok("Sent to " + client->target).dump());
        cout << client->name << " -> " << client->target << ": " << text << std::endl;
    }

    void ext(shared_ptr<Client>& client) {
         client->send(tools::ok("Goodbye").dump());
    }

} // namespace TENTA