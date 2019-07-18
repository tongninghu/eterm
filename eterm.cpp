#include <iostream>
#include <iomanip>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


#include "eterm.h"

using namespace std;

eterm::eterm() {
    count = 0;
}

vector<flights> eterm::AV(struct search_criteria &s) {
    vector<flights> reply;

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "115823");
        /* Connect to the MySQL test database */
        con->setSchema("eterm");

        pstmt = con->prepareStatement("SELECT * FROM Flights WHERE \
            (dep) = (?) AND (des) = (?) AND (departure_year) = (?) AND \
            (departure_month) = (?) AND (departure_date) = (?)");

        pstmt->setString(1, s.dep);
        pstmt->setString(2, s.des);
        pstmt->setInt(3, s.year);
        pstmt->setInt(4, s.month);
        pstmt->setInt(5, s.day);

        res = pstmt->executeQuery();
        while (res->next()) {
            struct flights f;
            f.flight = res->getString("flight");
            f.dep = res->getString("dep");
            f.des = res->getString("des");
            f.d_day = res->getInt("departure_date");
            f.d_month = res->getInt("departure_month");

            reply.push_back(f);
        }
        delete res;
        delete pstmt;
        delete con;

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "
           << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
           " )" << endl;
    }

    return reply;
}
