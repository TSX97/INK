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

int Database::get_or_create_private_chat(int user1_id, int user2_id) {
    if (!conn) return -1;
    string sql = "SELECT cm1.chat_id "
                 "FROM chat_members cm1 "
                 "JOIN chat_members cm2 ON cm1.chat_id = cm2.chat_id "
                 "WHERE cm1.user_id == " + to_string(user1_id) +
                 " AND cm2.user_id == " + to_string(user2_id);
    PGresult* res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQntuples(res) > 0) {
        int chat_id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        return chat_id;
    }
    PQclear(res);
    sql = "INSERT INTO chats (type) VALUES ('private') RETURNING id";
    res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return -1;
    }
    int new_chat_id = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    sql = "INSERT INTO chat_members (chat_id, user_id) VALUES"
          "(" + to_string(new_chat_id) + ", " + to_string(user1_id) + ")," +
          "(" + to_string(new_chat_id) + ", " + to_string(user2_id) + ")";
    PQexec(conn, sql.c_str());
    PQclear(res);
    return new_chat_id;

}
