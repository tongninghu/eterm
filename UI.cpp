#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include "UI.h"

using namespace std;

UI::UI(eterm &e):e(&e) {};

void UI::print(vector<string>& input) {
    for (int i = 1; i < input.size(); i++) {
        cout << i << ". " << input[i] << endl;
    }
}

void UI::print(vector<flights>& input) {
    for (int i = 0; i < input.size(); i++) {
        cout << i + 1 << ". ";
        input[i].print();
    }
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
            cout << "command = " << command << endl;
            cout << "arg = " << arg << endl;
            if (command == "AV") {
                if (!avTrigger(arg)) {
                    cout << "the input doesn't fit into command AV, please enter again"
                    << endl;
                }
            }
            else {
                cout << "no such command"
                << endl;
            }
        }
        else {
            cout << "the input doesn't fit into any commands, please enter again"
            << endl;
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

    reply = e->AV(s);
    if (reply.size() > 0) {
        print(reply);
    }
    else {
        cout << "No match records!" << endl;
    }

    return true;
}
