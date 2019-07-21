#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "PNR.h"


using namespace std;

void PNR::insert_id(const string & id) {
    this->id = id;
}

void PNR::insert_search(const flights a) {
    search.push_back(a);
}

void PNR::insert_flight(const flights& a) {
    f = a;
}

void PNR::insert_ts(time_t timeStamp) {
    ts = timeStamp;
}

void PNR::insert_name(const string& name) {
    this->name = name;
}

void PNR::insert_endtime(time_t endtime) {
    this->endtime = endtime;
}

void PNR::insert_ticketing(const string & ticketing) {
    this->ticketing = ticketing;
}

vector<flights>& PNR::getSearch() {
    return search;
}

flights PNR::getFlight() {
    return f;
}

string PNR::reply() {
    string reply;
    int c = 1;
    if (name != "") {
        reply += to_string(c++);
        reply += ". ";
        reply += name;
    }
    if (f.flight != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++);
        reply += ". ";
        reply += f.reply();
    }
    if (phone != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++);
        reply += ". ";
        reply += phone;
    }
    if (ticketing != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++);
        reply += ". ";
        reply += ticketing;
    }
    return reply;
}

time_t PNR::getEndtime() const{
    return endtime;
}

string PNR::getId() const{
    return id;
}

string PNR::getName() {
    return name;
}

string PNR::getTicketing() {
    return ticketing;
}
