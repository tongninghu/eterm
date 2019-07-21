#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>

#include <cstdio>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "eterm.h"

using namespace std;

eterm::eterm() {}

void eterm::socketCreation() {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((::bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    string id = to_string(cli.sin_addr.s_addr);
    id += to_string(cli.sin_port);
    // Function for chatting between client and server
		clientHandler(id, connfd);
		// After chatting close the socket
		close(sockfd);
}


void eterm::clientHandler(const string &id, int sockfd) {
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        string request (buff);
        bzero(buff, MAX);
        cout << "From client: " << request << endl;

        string reply = requestHanlder(id, request);
        strcpy(buff, reply.c_str());
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

string eterm::requestHanlder(const string &id, string request) {
    int end = request.find(' ');
    string command;
    string data;
    string reply;

    if (end < request.size() - 1) {
        command = request.substr(0, end);
        data = request.substr(end + 1);
        data.pop_back(); // erase the last charactor '\n'
        if (command == "AV") {
            if(!avTrigger(id, data, reply))
                reply = "The request doesn't fit into command AV, please enter again";
        }
        else if (command == "SD") {
            if(!sdTrigger(id, data, reply))
                reply = "The request doesn't fit into command SD, please enter again";
        }
        else if (command == "NM") {
            if(!nmTrigger(id, data, reply))
                reply = "The request doesn't fit into command NM, please enter again";
        }
        else if (command == "TK") {
            if(!tkTrigger(id, data, reply))
                reply = "The request doesn't fit into command TK, please enter again";
        }
        else {
            reply = "There is no command: ";
            reply += command;
            reply +=  ", please enter again";
        }
    }
    else {
        reply = "The request doesn't fit into any commands, please enter again";
    }
    return reply;
}


bool eterm::avTrigger(const string &id, string data, string &reply) {
    vector<string> temp;
    search_criteria s;

    auto start = 0;
    auto end = data.find("/");
    while (end != string::npos) {
        temp.push_back(data.substr(start, end - start));
        start = end + 1;
        end = data.find("/", start);
    }
    temp.push_back(data.substr(start));

    if (temp.size() == 0 || temp[0].size() != 6 || temp.size() == 1 ||
        (temp[1].size() < 4 && temp[1] != "+") || temp[1].size() > 5) {

        return false;
    }

    unordered_set<string>::iterator iter1 = cities.find(temp[0].substr(0, 3));
    unordered_set<string>::iterator iter2 = cities.find(temp[0].substr(3, 3));
    unordered_map<string, int>::iterator iter3 = months.find(temp[1].substr(temp[1].size() - 3, 3));

    if (iter1 == cities.end() || iter2 == cities.end() || iter3 == months.end()) {
        return false;
    }

    for(int i = 0; i < temp[1].size() - 3; i++) {
        if(!isdigit(temp[1][i])) {
            return false;
        }

        if (stoi(temp[1].substr(0, temp[1].size() - 3)) > 31) return false;
    }

    for (int j = 2; j < temp.size(); j++) {
        if (temp[j].size() == 4 && s.hour == -1) {
            for(int i = 0; i < 4; i++) {
                if(!isdigit(temp[j][i])) {
                    return false;
                }
            }
            if (stoi(temp[j].substr(0, 2)) > 24 || stoi(temp[j].substr(2, 2)) > 60) {
                return false;
            }
            s.hour = stoi(temp[j].substr(0, 2));
            s.min = stoi(temp[j].substr(2, 2));
        }
        else if (temp[j].size() == 2 && s.airline == "" && airlines.find(temp[j]) != airlines.end()) {
            s.airline = temp[j];
        }
        else {
            return false;
        }
    }

    s.dep = temp[0].substr(0, 3);
    s.des = temp[0].substr(3, 3);

    time_t now = time(0);

    if (temp[1] != "+") {
        tm *ltm = localtime(&now);
        s.year = 1900 + ltm->tm_year;
        s.month = months[temp[1].substr(temp[1].size() - 3, 3)];
        s.day = stoi(temp[1].substr(0, temp[1].size() - 3));
    }
    else {
        time_t t = now + 24 * 60 * 60;
        tm *ltm2 = localtime(&t);
        s.year = 1900 + ltm2->tm_year;
        s.month = 1 + ltm2->tm_mon;
        s.day = ltm2->tm_mday;
    }

    reply = AV(id, s);
    return true;
}


string eterm::AV(const string &id, struct search_criteria &s) {
    if (clients.find(id) == clients.end()) {
        PNR p;
        p.insert_id(id);
        clients.insert({id, p});
    }

    string reply;

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
        int c = 0;
        while (res->next()) {
            if (c++ == 0) clients[id].getSearch().clear();
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
            clients[id].insert_search(f);
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

    for (int i = 0; i < clients[id].getSearch().size(); i++) {
        reply += to_string(i + 1);
        reply += ". ";
        reply += clients[id].getSearch()[i].toString();
        reply += "\n";
    }
    reply.pop_back();
    return reply;
}

bool eterm::sdTrigger(const string &id, string data, string &reply) {
    if (clients.find(id) == clients.end() || clients[id].getSearch().size() == 0) {
        reply = "Please search first and then select";
        return true;
    }

    vector<string> temp;
    auto start = 0;
    auto end = data.find("/");
    while (end != string::npos) {
        temp.push_back(data.substr(start, end - start));
        start = end + 1;
        end = data.find("/", start);
    }
    temp.push_back(data.substr(start));

    string n;
    flights a = clients[id].getSearch()[stoi(temp[0]) - 1];

    if (temp[1] == "F") {n = a.F;}
    else if (temp[1] == "C") {n = a.C;}
    else if (temp[1] == "Y") {n = a.Y;}
    else if (temp[1] == "B") {n = a.B;}
    else if (temp[1] == "K") {n = a.K;}
    else {n = a.M;}

    if (stoi(temp[0]) <= clients[id].getSearch().size() && stoi(temp[2]) <= stoi(n)) {
        a.appointed_cabin = temp[1];
        a.appointed_num = stoi(temp[2]);
        time_t timeStamp = time(0);

        reply = SD(id, a, timeStamp);
        return true;
    }
    else {
        return false;
    }
}


string eterm::SD(const string &id, flights& a, time_t ts) {
    if (clients[id].getTicketing() != "") {
        string r = "You may not select flight now";
        return r;
    }

    if (current_threads.find(a) == current_threads.end()) {
        current_threads.insert(a);
    }
    else {
        string r = "Server busy, please try later";
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
            string r = "Already sold out, please choose another flight";
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

    clients[id].insert_flight(a);
    clients[id].insert_ts(ts);
    return clients[id].reply();
}

bool eterm::nmTrigger(const string &id, string data, string &reply) {
    reply = NM(id, data);
    return true;
}

string eterm::NM(const string &id, string &name) {
    if (clients.find(id) == clients.end()) {
        PNR p;
        p.insert_name(name);
        p.insert_id(id);
        clients.insert({id, p});
    }
    else if (clients[id].getTicketing() != "") {
        string r = "You may not change your name now";
        return r;
    }
    else {
        clients[id].insert_name(name);
    }
    return clients[id].reply();
}

bool eterm::tkTrigger(const string &id, string data, string &reply) {
    vector<string> temp;
    auto start = 0;
    auto end = data.find("/");
    while (end != string::npos) {
        temp.push_back(data.substr(start, end - start));
        start = end + 1;
        end = data.find("/", start);
    }
    temp.push_back(data.substr(start));

    time_t now = time(0);
    tm *ltm = localtime(&now);
    string year = to_string(1900 + ltm->tm_year);
    string hour = temp[1].substr(0, 2);
    string min = temp[1].substr(2, 2);
    string day = temp[2].substr(0, 2);
    string mon = to_string(months[temp[2].substr(2, 3)]);
    if (mon.size() == 1) {
        string s = "0" + mon;
        mon = s;
    }
    string endTime = year;
    endTime += "/";
    endTime += mon;
    endTime += "/";
    endTime += day;
    endTime += " ";
    endTime += hour;
    endTime += ":";
    endTime += min;
    endTime += ":";
    endTime += "00";

    time_t tEnd;
    int yy, month, dd, hh, mm, ss;
    struct tm whenEnd;
    const char *zEnd = endTime.c_str();

    sscanf(zEnd, "%d/%d/%d %d:%d:%d", &yy, &month, &dd, &hh, &mm, &ss);
    whenEnd.tm_year = yy - 1900;
    whenEnd.tm_mon = month - 1;
    whenEnd.tm_mday = dd;
    whenEnd.tm_hour = hh;
    whenEnd.tm_min = mm;
    whenEnd.tm_sec = ss;
    whenEnd.tm_isdst = -1;

    tEnd = mktime(&whenEnd);
    reply = TK(id, data, tEnd);
    return true;
}

string eterm::TK(const string &id, string &ticketing, time_t endtime) {
    if (clients.find(id) == clients.end()) {
        string r = "Please select a flight first";
        return r;
    }
    clients[id].insert_ticketing(ticketing);
    clients[id].insert_endtime(endtime);
    ticketingList.push(clients[id]);
    return clients[id].reply();
}

void eterm::PNR_maintain() {
  while (1) {
      time_t now = time(0);
      while (!ticketingList.empty() && ticketingList.top().getEndtime() < now) {
          string id = ticketingList.top().getId();
          clients.erase(id);
          ticketingList.pop();
          now = time(0);
      }
      this_thread::sleep_for(std::chrono::minutes(1));
  }
}
