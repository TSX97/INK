#include "db/database.hpp"
#include <string>
#include <iostream>
#include <libpq-fe.h>
#include "crypto/password.hpp"

using namespace std;


Database::Database(const string& connection_setting) {
    conn = PQconnectdb(connection_setting.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "DB error: " << PQerrorMessage(conn) << endl;
        conn = nullptr;
    }
}
Database::~Database() {
    if (conn) PQfinish(conn);
}

bool Database::create_user(const string& name, const string& password) {
    if (!conn) return false;
    string hashed = crypto::hash_password(password);
    cout << "DEBUG: hashed password = " << hashed << endl;
    string sql = "INSERT INTO users (name, password_hash) VALUES ('" + name + "', '" + hashed + "')";
    PGresult* res = PQexec(conn, sql.c_str());
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

int Database::authenticate(const string& name, const string& password) {
    if (!conn) return -1;
    string sql = "SELECT id, password_hash FROM users WHERE name = '" + name + "'";
    PGresult* res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return -1;
    }
    string stored_hash = PQgetvalue(res, 0, 1);
    int id = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    cout << "DEBUG: stored hash = " << stored_hash << endl;
    return crypto::verify_password(password, stored_hash) ? id : -1;
}

int Database::find_user(const string& name) {
    if (!conn) return -1;
    string sql = "SELECT * "
                 "FROM users "
                 "WHERE name = '" + name + "'";
    PGresult* res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return -1;
    }
    int id = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return id;
}

std::vector<std::pair<int, std::string>> Database::get_all() {
    std::vector<std::pair<int, std::string>> result;
    if (!conn) return result;

    PGresult* res = PQexec(conn, "SELECT id, name FROM users ORDER BY name");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return result;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        int id = atoi(PQgetvalue(res, i, 0));
        std::string name = PQgetvalue(res, i, 1);
        result.push_back({id, name});
    }
    PQclear(res);
    return result;
}