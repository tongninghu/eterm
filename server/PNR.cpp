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

void PNR::insert_searchPrice(vector<string> p) {
    this->searchPrice.push_back(p);
}

void PNR::insert_price(int num) {
    string currency = searchPrice[num - 1][0];
    string fcny = searchPrice[num - 1][1];
    string tcny_cn = searchPrice[num - 1][2];
    string tcny_yq = searchPrice[num - 1][3];
    string total = searchPrice[num - 1][4];
    string xcny = to_string(stoi(tcny_cn) + stoi(tcny_yq));

    fp += currency;
    fc += f.dep + " " + f.airline + " " + f.des + " " + total + f.appointed_cabin\
       + " " +  currency + total + " END";
    fn += "FCNY" + fcny + "/SCNY" + fcny + "/" + f.appointed_cabin\
       + to_string(f.appointed_num) + "/XCNY" + xcny + "/TCNY" + tcny_cn + "CN/TCNY"\
       + tcny_yq + "YQ/ACNY" + total;
}


vector<flights>& PNR::getSearch() {
    return search;
}

flights PNR::getFlight() {
    return f;
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

vector<vector<string>>& PNR::getSearchPrice() {
    return searchPrice;
}



string PNR::reply() {
    string reply;
    int c = 1;
    if (name != "") {
        reply += to_string(c++) + ". " + name;
    }
    if (f.flight != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++) + ". " + f.reply();
    }
    if (phone != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++) + ". " + phone;
    }
    if (ticketing != "") {
        if (c > 1) reply += "\n";
        reply += to_string(c++) + ". " + ticketing;
    }
    if (fp != "FP/CASH ") {
        reply += "\n";
        reply += to_string(c++) + ". " + fp + "\n" + to_string(c++) + ". " + fc\
              + "\n" + to_string(c++) + ". " + fn;
    }
    return reply;
}
