#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include "UI.h"

using namespace std;

UI::UI(eterm &e):e(&e) {
    gen_random(mac_a, 32);  // plan to use mac address, but fail to find it on mac os
};

void UI::gen_random(string &s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
    }
}

void UI::print(vector<string>& input) {
    cout << endl;
    for (int i = 0; i < input.size(); i++) {
        cout << i + 1 << ". " << input[i] << endl;
    }
    cout << endl;
}

void UI::print(vector<flights>& input) {
    cout << endl;
    for (int i = 0; i < input.size(); i++) {
        cout << i + 1 << ". ";
        input[i].print();
    }
    cout << endl;
}

void UI::getCommand() {
    string input;
    while (getline(cin, input)) {
        int end = input.find(' ');
        string command;
        string arg;
        if (end < input.size() - 1) {
            command = input.substr(0, end);
            arg = input.substr(end + 1);
            if (command == "AV") {
                if (!avTrigger(arg)) {
                    cout << "the input doesn't fit into command AV, please enter again"
                    << endl << endl;
                }
            }
            else if (command == "SD") {
                if (!sdTrigger(arg)) {
                    cout << "the input doesn't fit into command SD, please enter again"
                    << endl << endl;
                }
            }
            else if (command == "NM") {
                nmTrigger(arg);
            }
            else if (command == "TK") {
                tkTrigger(arg);
            }
            else {
                cout << "There is no command: " << command << ", please enter again"
                << endl << endl;
            }
        }
        else {
            cout << "the input doesn't fit into any commands, please enter again"
            << endl << endl;
        }
    }
}

bool UI::avTrigger(string arg) {
    vector<string> temp;
    search_criteria s;

    auto start = 0;
    auto end = arg.find("/");
    while (end != string::npos) {
        temp.push_back(arg.substr(start, end - start));
        start = end + 1;
        end = arg.find("/", start);
    }
    temp.push_back(arg.substr(start));

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

    search = e->AV(s);
    if (search.size() > 0) {
        print(search);
    }
    else {
        cout << "No match records!" << endl;
    }
    return true;
}


bool UI::sdTrigger(string arg) {
    vector<string> temp;
    auto start = 0;
    auto end = arg.find("/");
    while (end != string::npos) {
        temp.push_back(arg.substr(start, end - start));
        start = end + 1;
        end = arg.find("/", start);
    }
    temp.push_back(arg.substr(start));

    string n;
    if (temp[1] == "F") {n = search[stoi(temp[0])].F;}
    else if (temp[1] == "C") {n = search[stoi(temp[0])].C;}
    else if (temp[1] == "Y") {n = search[stoi(temp[0])].Y;}
    else if (temp[1] == "B") {n = search[stoi(temp[0])].B;}
    else if (temp[1] == "K") {n = search[stoi(temp[0])].K;}
    else {n = search[stoi(temp[0])].M;}

    if (stoi(temp[0]) <= search.size() && stoi(temp[2]) <= stoi(n)) {
        search[stoi(temp[0]) - 1].appointed_cabin = temp[1];
        search[stoi(temp[0]) - 1].appointed_num = stoi(temp[2]);
        time_t timeStamp = time(0);
        reply = e->SD(search[stoi(temp[0]) - 1], mac_a, timeStamp);
        if (reply[0] == "fail") {
            cout << "server busy, please try later" << endl;
            return false;
        }
        else if (reply[0] == "sold out") {
            cout << "already sold out, please find another flight" << endl;
            return false;
        }
        print(reply);
        return true;
    }
    else {
        return false;
    }
}

bool UI::nmTrigger(string arg) {
    reply = e->NM(arg, mac_a);
    print(reply);
    return true;
}

bool UI::tkTrigger(string arg) {
    vector<string> temp;
    auto start = 0;
    auto end = arg.find("/");
    while (end != string::npos) {
        temp.push_back(arg.substr(start, end - start));
        start = end + 1;
        end = arg.find("/", start);
    }
    temp.push_back(arg.substr(start));

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
    reply = e->TK(arg, tEnd, mac_a);
    if (reply[0] == "no record") {
        cout << "please select a flight first" << endl;
        return false;
    }
    print(reply);
    return true;
}
