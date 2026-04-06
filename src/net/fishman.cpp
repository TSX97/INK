#include "net/fishman.hpp"
#include "net/protocol.hpp"
#include "tools.hpp"
#include <map>
#include <functional>
#include <db/database.hpp>

using namespace std;

using json = nlohmann::json;

namespace Fishman {
    using Handler = function<void(const nlohmann::json&, shared_ptr<Client>& client, ClientManager& cm, Database& db)>;
    map<string, Handler> handlers;

    void init() {

        handlers["REG"] = [](const json& j, auto client, auto& cm, Database& db) {
            std::cout << "DEBUG: j = " << j.dump() << std::endl;
            std::string name = tools::get_arg(j, "name");
            std::cout << "DEBUG: name = '" << name << "'" << std::endl;
            TENTA::reg(name, client, cm, db);
        };

        handlers["AUT"] = [](const json& j, auto client, auto& cm, Database& db) {
            string name = tools::get_arg(j, "name");
            string password = tools::get_arg(j, "password");
            TENTA::aut(name, password, client, db);
        };

        handlers["BND"] = [](const json& j, std::shared_ptr<Client> client, ClientManager& cm, Database& db) {
            string name = tools::get_arg(j, "target");
            TENTA::bnd(name, client, cm);
        };

        handlers["MSG"] = [](const json& j, std::shared_ptr<Client> client, ClientManager& cm, Database& db) {
            string text = tools::get_arg(j, "text");
            TENTA::msg(text, client, cm);
        };

        handlers["LST"] = [](const json& j, std::shared_ptr<Client> client, ClientManager& cm, Database& db) {
            TENTA::lst(client, cm, db);
        };

        handlers["EXT"] = [](const json& j, std::shared_ptr<Client> client, ClientManager& cm, Database& db) {
            TENTA::ext(client);
        };
    }

    void handle(const string& raw, std::shared_ptr<Client> client, ClientManager& cm, Database& db) {
        auto j = tools::parse_json(raw);
        string cmd = tools::get_arg(j, "cmd");

        std::cout << "DEBUG: cmd = " << cmd << std::endl;

        auto it = handlers.find(cmd);
        if (it != handlers.end()) {
            it->second(j, client, cm, db);
        } else {
            client->send(tools::error("Unknown command: " + cmd).dump());
        }
    }
}