#include "net/protocol.hpp"
#include "db/database.hpp"
#include "tools.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace TENTA {
    void reg(const std::string& name, std::shared_ptr<Client>& client, ClientManager& cm, Database& db) {
        if (name.empty()) {
            client->send(tools::error("name cannot be empty").dump());
            return;
        }

        if (db.find_user(name) != -1) {
            client->send(tools::error("user already exists").dump());
            return;
        }

        if (!db.create_user(name, "temp")) {
            client->send(tools::error("db error").dump());
            return;
        }

        client->name = name;
        client->send(tools::ok("Registered as " + name).dump());
        cm.add(client);
        cout << "User " << client->name << " registered" << std::endl;
    }

    void aut(const string& name, const string& password, shared_ptr<Client>& client, Database& db) {
        int user_id = db.authenticate(name, password);
        if (user_id == -1) {
            client->send(tools::error("Invalid credentials").dump());
            return;
        }
        client->user_id = user_id;
        client->name = name;
        client->send(tools::ok("Authenticated as " + name).dump());
        cout << "user " << client->name << " (id=" << user_id << ") authenticated" << endl;
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

    void lst(shared_ptr<Client>& client, ClientManager& cm) {
        vector<shared_ptr<Client>> clients = cm.get_all();
        if (clients.empty()) {
            return;
        }
        vector<string> names;
        for (const auto& c : clients) {
            if (c->name == client->target) {
                names.push_back("*" + c->name);
                continue;
            }
            if (c->name == client->name) {
                continue;
            }
            names.push_back(c->name);
        }
        client->send("\n" + tools::join(names, "\n"));
    }

    void ext(shared_ptr<Client>& client) {
         client->send(tools::ok("Goodbye").dump());
    }

} // namespace TENTA