#include "net/server.hpp"
#include "db/database.hpp"
using namespace std;

int main() {
    Database db("dbname=ink user=postgres password=143 host=localhost");
    Server server(8080, db);
    server.run();
    return 0;
}

