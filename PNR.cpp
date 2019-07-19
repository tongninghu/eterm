#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "PNR.h"


using namespace std;

void PNR::insert_flight(const flights& a) {
    f = a;
}

void PNR::insert_ts(time_t timeStamp) {
    ts = timeStamp;
}

void PNR::insert_name(const string& name) {
    this->name = name;
}

void PNR::insert_mac(const string & mac_a) {
    this->mac_a = mac_a;
}

void PNR::insert_endtime(time_t endtime) {
    this->endtime = endtime;
}

void PNR::insert_ticketing(const string & ticketing) {
    this->ticketing = ticketing;
}

vector<string> PNR::reply() {
    vector<string> reply;
    if (name != "") reply.push_back(name);
    if (f.flight != "") reply.push_back(f.reply());
    if (phone != "") reply.push_back(phone);
    if (ticketing != "") reply.push_back(ticketing);
    return reply;
}

time_t PNR::getEndtime() const{
    return endtime;
}

string PNR::getMacAddress() const{
    return mac_a;
}
/*
bool operator<(const PNR& l, const PNR& r) {
    if (l.endtime < r.endtime) return true;
    else return false;
} */
