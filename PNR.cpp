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

vector<string> PNR::reply() {
    vector<string> reply;
    if (name != "") reply.push_back(name);
    if (f.flight != "") reply.push_back(f.reply());
    if (phone != "") reply.push_back(phone);
    return reply;
}
