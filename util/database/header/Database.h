#include <mysql_driver.h>
#include <mysql_connection.h>

class Database{

    public:
    void set_database(string dbname);
    void select(string query);

}