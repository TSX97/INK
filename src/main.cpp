#include "net/server.hpp"
#include "db/database.hpp"
using namespace std;

int main() {
    Database db("dbname=ink user=postgres password=your_password");
    Server server(8080, "dbname=ink user=postgres password=your_password");
    server.run();
    return 0;
}