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

eterm::eterm() {}

vector<flights> eterm::AV(struct search_criteria &s) {
    vector<flights> reply;

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "115823");
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
            f.F = to_string(res->getInt("F"));
            f.C = to_string(res->getInt("C"));
            f.Y = to_string(res->getInt("Y"));
            f.B = to_string(res->getInt("B"));
            f.K = to_string(res->getInt("K"));
            f.M = to_string(res->getInt("M"));
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

vector<string> eterm::SD(flights a, string mac_a,  time_t ts) {
    if (current_threads.find(a) == current_threads.end()) {
        current_threads.insert(a);
    }
    else {
        vector<string> r;
        r.push_back("fail");
        return r;
    }

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        int available;
        int lock;
        string k = a.appointed_cabin + "L";

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "115823");
        con->setSchema("eterm");

        pstmt = con->prepareStatement("SELECT * FROM Flights WHERE (flight) =\
            (?) AND (departure_month) = (?) AND (departure_date) = (?)");

        pstmt->setString(1, a.flight);
        pstmt->setInt(2, a.d_month);
        pstmt->setInt(3, a.d_day);

        res = pstmt->executeQuery();

        while (res->next()) {
            available = res->getInt(a.appointed_cabin);
            lock = res->getInt(k);
        }

        if (available == 0) {
            vector<string> r;
            r.push_back("sold out");
            return r;
        }

        if (a.appointed_cabin == "F") {
            pstmt = con->prepareStatement("UPDATE Flights SET FL = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }
        else if (a.appointed_cabin == "C") {
            pstmt = con->prepareStatement("UPDATE Flights SET CL = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }
        else if (a.appointed_cabin == "Y") {
            pstmt = con->prepareStatement("UPDATE Flights SET YL = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }
        else if (a.appointed_cabin == "B") {
            pstmt = con->prepareStatement("UPDATE Flights SET BL = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }
        else if (a.appointed_cabin == "K") {
            pstmt = con->prepareStatement("UPDATE Flights SET KL = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }
        else {
            pstmt = con->prepareStatement("UPDATE Flights SET ML = (?)\
              WHERE (flight) = (?) AND (departure_month) = (?) AND \
              (departure_date) = (?)");
        }

        pstmt->setInt(1, lock + 1);
        pstmt->setString(2, a.flight);
        pstmt->setInt(3, a.d_month);
        pstmt->setInt(4, a.d_day);

        res = pstmt->executeQuery();

        delete res;
        delete pstmt;
        delete con;
        current_threads.erase(a);

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "
           << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
           " )" << endl;
    }
    if (clients.find(mac_a) == clients.end()) {
        PNR p;
        p.insert_flight(a);
        p.insert_ts(ts);
        clients.insert({mac_a, p});
    }
    else {
        clients[mac_a].insert_flight(a);
        clients[mac_a].insert_ts(ts);
    }

    return clients[mac_a].reply();
}

vector<string> eterm::NM(string name, string mac_a) {
    if (clients.find(mac_a) == clients.end()) {
        PNR p;
        p.insert_name(name);
        clients.insert({mac_a, p});
    }
    else {
        clients[mac_a].insert_name(name);
    }
    return clients[mac_a].reply();
}
