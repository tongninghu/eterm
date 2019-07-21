#include <iostream>
#include <string>
#include "flights.h"

using namespace std;

flights::flights() {
    appointed_cabin = "";
}

flights& flights::operator=(const flights& a) {
    flight = a.flight;
    dep = a.dep;
    des = a.des;
    d_day = a.d_day;
    d_month = a.d_month;
    airline = a.airline;
    F = a.F;
    C = a.C;
    Y = a.Y;
    B = a.B;
    K = a.K;
    M = a.M;
    appointed_cabin = a.appointed_cabin;
    appointed_num = a.appointed_num;
    return *this;
}

bool operator==(const flights& l, const flights& r) {
    if (l.flight != r.flight || l.d_day != r.d_day || l.d_month != r.d_month ||\
        l.appointed_cabin != r.appointed_cabin ) return false;
    return true;
}

string flights::toString() const{
    string reply;
    reply += flight;
    reply += " ";
    reply += dep;
    reply += " ";
    reply += des;
    reply += " ";
    reply += to_string(d_day);
    reply += " ";
    reply += to_string(d_month);
    reply += " ";
    reply += airline;
    reply += " F";
    reply += F;
    reply += " C";
    reply += C;
    reply += " Y";
    reply += Y;
    reply += " B";
    reply += B;
    reply += " K";
    reply += K;
    reply += " M";
    reply += M;
    return reply;
}

string flights::reply() {
    string reply;
    reply += flight;
    reply += " ";
    reply += dep;
    reply += " ";
    reply += des;
    reply += " ";
    reply += to_string(d_day);
    reply += " ";
    reply += to_string(d_month);
    reply += " ";
    reply += appointed_cabin;
    reply += " ";
    reply += to_string(appointed_num);
    return reply;
}
