#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <libpq-fe.h>

class Database {
public:
	Database(const std::string& connection_setting);
	~Database();
	bool create_user(const std::string& name, const std::string& password);
	int find_user(const std::string& name);
	int authenticate(const std::string& name, const std::string& password);
	void save_message(int from, int to, const std::string& text);

private:
	PGconn* conn;

};
#endif
